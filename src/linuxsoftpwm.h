//
// Created by vlad on 5/23/25.
//

#ifndef LINUXSOFTPWM_H
#define LINUXSOFTPWM_H
#include <gpiod.h>
#include <thread>
#include <unordered_map>
#include <mutex>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <string>
#include <vector>
#include <stdexcept>
namespace linuxsoftpwm {

    struct PwmState {
        std::atomic<double> frequency;
        std::atomic<double> duty;
        std::atomic<bool> active;
        std::thread worker;
    };

    struct ReadState {
        std::atomic<double> frequency;
        std::atomic<double> duty;
        std::atomic<bool> valid;
        std::thread worker;
    };
    void pwm_loop(const std::string& chipname, int line_num, std::atomic<double>& freq, std::atomic<double>& duty, std::atomic<bool>& active);
    void reader_loop(const std::string& chipname, int line_num, std::atomic<double>& freq, std::atomic<double>& duty, std::atomic<bool>& valid);
    void WritePWM(const std::string& chipname, int line, double frequency, double duty_cycle);
    std::pair<double, double> ReadPWM(const std::string& chipname, int line);
    static std::mutex pwm_mutex;
    static std::unordered_map<std::string, std::unordered_map<int, PwmState>> pwm_outputs;
    static std::unordered_map<std::string, std::unordered_map<int, ReadState>> pwm_inputs;
}
#endif //LINUXSOFTPWM_H
