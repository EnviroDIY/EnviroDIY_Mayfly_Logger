Initial Demo Sketch: Blink & Serial Temperature
==============

All of the Mayfly boards have been pre-programmed with this initial demonstration sketch.  The sketch is provided here so that you can understand it and reload it if overwrite it while  trying out other examples.

This sketch uses a library called *Sodaq_DS3231.h*, which is necessary to interact with the Real Time Clock's DS3231 RTC chip.  The Mayfly is based on the SODAQ Mbili, so the SODAQ\_DS3231 library is recommended. 

In order to load this sketch, you first need to install the SODAQ\_DS3231 library, either by:

*  using the Arudino IDE from this menu utility: Skectch > Include Library > Manage Libraries...; or by 
* directly obtaining the library from: [https://github.com/SodaqMoja/Sodaq_DS3231](https://github.com/SodaqMoja/Sodaq_DS3231)


### What this sketch does:
When you first connect power to the board, and turn the board on (switch on upper left) you will see the red and green LEDs blinking.

You can also see the real-time temperature of your Mayfly board if you [connect the Mayfly to your computer and configure the Arduino IDE to communicate with it](https://github.com/EnviroDIY/Arduino_boards). Once the Arduino IDE is configured to communicate with your Mayflay board, open the [Serial Monitor button (button on upper right of any sketch)](https://lh6.googleusercontent.com/GO9HQ1q3v4ho-H7ZqP55cQ4o_nLdyYpkCauIWOvN5xrQAMNIfgeiu_LiRTfAN2yruvjBLGMNrACzWffwhlM5ADSem35dDPpI9Mj5WWN-l8YSizSh-3HwvPEwtzAo3o0ZZjJgAyw), and change the serial communication rate at 57600 baud. You will see the current board temperature as measured by the DS3231 Real Time Clock chip's internal temperature sensor, updated every second.
