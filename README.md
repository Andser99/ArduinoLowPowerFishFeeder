# ArduinoLowPowerFishFeeder
A low power fish feeder framework for timing code using the watchdog.

This system allows the arduino to go to sleep for 8 seconds and then process some important code.

During sleep mode, the arduino draws ~0.8mA using a Nano 3.0 Atmega168 version. This board tested also had its VCC removed and was wired using the 5V pin and Ground through a 5V supply. After testing the function of the VCC, it was found that with regular usage (no sleep modes, low load) it draws 15.95mA with only an empty loop code running, then 18.50mA with a LED connected to it. However, when removed, this increased to 23mA with the empty loop code and 24.47mA with the same LED connected.
The LED was connected with a 220 ohm resistor.

LEDs such as the power LED located on the arduino board should be removed to further decrease the necessary current.

### Example usage
DEBUG def should be set to false to remove power usage from serial messaging (although the impact is minimal, ~5% increase in power consumption). feedingTicksRequired should be set to the desired feeding interval (HoursToTicks(<x>) converts hours to a tick period that fits in with the 8 second sleep period. Pin D2 can be connected to an LED so its easier to find out whether the program is running or dead, this will create a small flash every 8 seconds.
