Mayfly Data Logger Software
==============
To program the EnviroDIY Mayfly, you can use the free Arduino IDE programming software.  Download the latest version from Arduino.cc.  If you already have the IDE installed on your computer, ensure that you are using version 1.6.5 or newer.

Once the Arduino IDE is installed, you will need to add the Mayfly board to the list of available boards.  To do this, start the Arduino software, then click on the “File” menu, then “Preferences” and paste the following URL into the box labeled “Additional Boards Manager URLs”:

```
https://raw.githubusercontent.com/EnviroDIY/Arduino_boards/master/package_EnviroDIY_index.json
```

Then back in the IDE, click on Tools > Board > Boards Manager. Use the dropdown box to select “Contributed”. You should then see an option for “EnviroDIY ATmega Boards”. Click the Install button to add the EnviroDIY boards to your IDE.

Now when you click Tools > Board you will see the EnviroDIY Mayfly 1284P listed either at the top or bottom of your list of available boards.

## Sample Sketch

All of the Mayfly boards have been pre-programmed with a sample sketch.  When you first connect power to the board, you will see the red and green LEDs blinking.  If you connect the Mayfly to your computer and open a serial terminal window at 57600 baud, you will see the current temperature as measured by the DS3231’s internal temperature sensor, updated every second.

This sketch uses a library called Sodaq_DS3231.h which is necessary to interact with the DS3231 RTC chip.  The Mayfly is based on the SODAQ Mbili, so the SODAQ DS3231 library is recommended.

```cpp
#include <Wire.h>
#include "Sodaq_DS3231.h"

int State8 = LOW;
int State9 = LOW;

int LEDtime = 1000;   //milliseconds

void setup ()
{
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    Serial.begin(57600);
    Wire.begin();
    rtc.begin();

    Serial.println("EnviroDIY Mayfly: Blink demo with serial temperature");     
}

void loop ()
{
    if (State8 == LOW) {
      State8 = HIGH;
    } else {
      State8 = LOW;
    }
    digitalWrite(8, State8);
    State9 = !State8;
    digitalWrite(9, State9);

    rtc.convertTemperature();             //convert current temperature into registers
    Serial.print(rtc.getTemperature(),2); //read registers and display the temperature
    Serial.println("deg C");

    delay(LEDtime);
}
```

## Other sketches

- Blinking LEDs
- [Battery measurement](https://github.com/EnviroDIY/EnviroDIY_Mayfly_Logger/tree/master/examples/battery_measurement)
- Simple file example
- Digital humidity/temperature sensor (with switched 3.3v power)
- Serial communication
- Ultrasonic sensor
- Sleeping Mayfly logger
