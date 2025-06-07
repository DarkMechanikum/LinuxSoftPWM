#include "linuxsoftpwm.h"
namespace linuxsoftpwm {

void pwm_loop(const std::string& chipname, int line_num, std::atomic<double>& freq, std::atomic<double>& duty, std::atomic<bool>& active) {
    gpiod_chip* chip = gpiod_chip_open_by_name(chipname.c_str());
    gpiod_line* line = gpiod_chip_get_line(chip, line_num);
    gpiod_line_request_output(line, "linuxsoftpwm-writer", 0);

    while (active.load()) {
        double f = freq.load();
        double d = duty.load();

        if (f <= 0 || d < 0 || d > 100) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        std::chrono::duration<double> period(1.0 / f);
        auto high = period * (d / 100.0);
        auto low = period - high;

        gpiod_line_set_value(line, 1);
        std::this_thread::sleep_for(std::chrono::duration_cast<std::chrono::microseconds>(high));
        gpiod_line_set_value(line, 0);
        std::this_thread::sleep_for(std::chrono::duration_cast<std::chrono::microseconds>(low));
    }

    gpiod_line_release(line);
    gpiod_chip_close(chip);
}

void reader_loop(const std::string& chipname, int line_num, std::atomic<double>& freq, std::atomic<double>& duty, std::atomic<bool>& valid) {
    gpiod_chip* chip = gpiod_chip_open_by_name(chipname.c_str());
    gpiod_line* line = gpiod_chip_get_line(chip, line_num);
    gpiod_line_request_both_edges_events(line, "linuxsoftpwm-reader");

    std::chrono::steady_clock::time_point last_rising, falling;
    bool first = true;

    while (true) {
        struct gpiod_line_event ev;
        if (gpiod_line_event_wait(line, nullptr) > 0) {
            gpiod_line_event_read(line, &ev);
            auto now = std::chrono::steady_clock::now();

            if (ev.event_type == GPIOD_LINE_EVENT_RISING_EDGE) {
                if (!first) {
                    double period = std::chrono::duration<double>(now - last_rising).count();
                    double high = std::chrono::duration<double>(falling - last_rising).count();
                    freq.store(1.0 / period);
                    duty.store((high / period) * 100.0);
                    valid.store(true);
                }
                last_rising = now;
                first = false;
            } else if (ev.event_type == GPIOD_LINE_EVENT_FALLING_EDGE) {
                falling = now;
            }
        }
    }
}

void WritePWM(const std::string& chipname, int line, double frequency, double duty_cycle) {
    std::lock_guard<std::mutex> lock(pwm_mutex);
    PwmState& state = pwm_outputs[chipname][line];
    if (!state.active.load()) {
        state.frequency = frequency;
        state.duty = duty_cycle;
        state.active = true;
        state.worker = std::thread(pwm_loop, chipname, line, std::ref(state.frequency), std::ref(state.duty), std::ref(state.active));
    } else {
        state.frequency = frequency;
        state.duty = duty_cycle;
    }
}

std::pair<double, double> ReadPWM(const std::string& chipname, int line) {
    std::lock_guard<std::mutex> lock(pwm_mutex);
    ReadState& state = pwm_inputs[chipname][line];
    if (!state.worker.joinable()) {
        state.valid = false;
        state.worker = std::thread(reader_loop, chipname, line, std::ref(state.frequency), std::ref(state.duty), std::ref(state.valid));
    }
    if (!state.valid.load()) return {0.0, 0.0};
    return {state.frequency.load(), state.duty.load()};
}

}
