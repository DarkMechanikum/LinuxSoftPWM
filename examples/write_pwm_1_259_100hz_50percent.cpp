#include <linuxsoftpwm.h>  // The header you installed to /usr/local/include
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    const std::string chip = "gpiochip1";
    const int line = 259;
    const double frequency = 100.0;     // Hz
    const double duty_cycle = 50.0;     // percent

    std::cout << "Starting PWM on " << chip << ":" << line
              << " @ " << frequency << "Hz, " << duty_cycle << "% duty" << std::endl;

    // Start PWM
    linuxsoftpwm::WritePWM(chip, line, frequency, duty_cycle);

    // Keep the program alive for 10 seconds
    std::this_thread::sleep_for(std::chrono::seconds(30));

    std::cout << "PWM still running in background thread. Exiting now." << std::endl;
    return 0;
}
