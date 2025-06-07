# General
The first two numbers in the name of the example represent the gpiochip number and the line number of that chip. You can get that information from the gpiod library and datasheet of your board. 
## Build
To build the example please make sure, that you have linuxsoftpwm installed to /usr/local/lib
Build the example, please change "example.cpp" to the actual example source you wish to build `g++ -std=c++17 -o example exmaple.cpp -L /usr/local/lib -llinuxsoftpwm -lgpiod`
## Run 
If libgpiod requires sudo access, running the example without sudo will lead to segmentation fault.
`sudo ./example`
