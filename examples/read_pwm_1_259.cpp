#include <linuxsoftpwm.h>  // Provided by your installed library
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    const std::string chip = "gpiochip1";
    const int line = 259;

    std::cout << "Starting PWM reader on " << chip << ":" << line << std::endl;

    while (true) {
        auto [freq, duty] = linuxsoftpwm::ReadPWM(chip, line);

        std::cout << "Frequency: " << freq << " Hz, "
                  << "Duty Cycle: " << duty << " %" << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return 0;
}
