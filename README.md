# LinuxSoftPWM
This library allows to use PWM on any pin, avalibale from libgpiod. 
It is not even remotely accurate due to inavitable latencies due to running from linux userspace. However, it is still suitable for some extremely basic applications. 

To build the library:
1. `mkdir build`
2. `cd build`
3. `cmake ..`
4. `make`
5. `sudo make install`

Please see the example directory and the corresponding readme file.
