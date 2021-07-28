# ArduinoLowPowerFishFeeder
A low power fish feeder framework for timing code using the watchdog.

This system allows the arduino to go to sleep for 8 seconds and then process some important code.

During sleep mode, the arduino draws ~0.8mA using a Nano 3.0 Atmega168 version. This board tested also had its VCC removed and was wired using the 5V pin and Ground through a 5V supply. After testing the function of the VCC, it was found that with regular usage (no sleep modes, low load) it draws 15.95mA with only an empty loop code running, then 18.50mA with a LED connected to it. However, when removed, this increased to 23mA with the empty loop code and 24.47mA with the same LED connected.
The LED was connected with a 220 ohm resistor.
