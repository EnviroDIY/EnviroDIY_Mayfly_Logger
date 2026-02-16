#include <Wire.h>
#include "Sodaq_DS3231.h"         

int State8 = LOW;
int State9 = LOW;

int LEDtime = 1000;   //milliseconds

void setup () 
{
    Serial.begin(57600);
    Wire.begin();
    rtc.begin();
    Serial.println("EnviroDIY Mayfly: Blink demo with serial temperature");
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);    
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
    Serial.println(" deg C");
   
    delay(LEDtime);
}
