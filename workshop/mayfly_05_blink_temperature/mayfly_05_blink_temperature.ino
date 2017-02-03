/**************************************************************************

* Written By:  Shannon Hicks <shicks@stroudcenter.org>
* Documentation By: Anthony Aufdenkampe <aaufdenkampe@limno.com>
* Creation Date: 2016
* Development Environment: Arduino IDE 1.6.5+
* Hardware Platform: Stroud Center, EnviroDIY Mayfly Arduino Datalogger

All of the Mayfly boards have been pre-programmed with this initial 
demonstration sketch. This sketch uses a library called *Sodaq_DS3231.h*, 
which is necessary to interact with the Real Time Clock's DS3231 RTC chip. 
In order to load this sketch, you first need to install the SODAQ_DS3231 
library, either by: using the Arudino IDE from this menu utility: 
Skectch > Include Library > Manage Libraries...; or by 
directly obtaining the library from: 
https://github.com/SodaqMoja/Sodaq_DS3231

**************************************************************************/

#include <Wire.h>           // This library is included with the Arduino IDE, and allows communication with I2C/TWI devices
#include "Sodaq_DS3231.h"   // Install this library to interact with the Real Time Clock

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
