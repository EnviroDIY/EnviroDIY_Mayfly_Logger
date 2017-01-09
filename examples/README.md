Example Arduino Sketches for the Mayfly Board
==============

All of the Mayfly boards have been pre-programmed with an initial demonstration sketch, named "initial\_demo\_blink_temperature".  When you first connect power to the board, and turn the board on (switch on upper left) you will see the red and green LEDs blinking.

You can also see the real-time temperature of your Mayfly board if you [connect the Mayfly to your computer and configure the Arduino IDE to communicate with it](https://github.com/EnviroDIY/Arduino_boards). Once the Arduino IDE is configured to communicate with your Mayflay board, open the [Serial Monitor button (button on upper right of any sketch)](https://lh6.googleusercontent.com/GO9HQ1q3v4ho-H7ZqP55cQ4o_nLdyYpkCauIWOvN5xrQAMNIfgeiu_LiRTfAN2yruvjBLGMNrACzWffwhlM5ADSem35dDPpI9Mj5WWN-l8YSizSh-3HwvPEwtzAo3o0ZZjJgAyw), and change the serial communication rate at 57600 baud. You will see the current board temperature as measured by the DS3231 Real Time Clock chip's internal temperature sensor, updated every second.

Try out some of the other examples sketches shared here!
