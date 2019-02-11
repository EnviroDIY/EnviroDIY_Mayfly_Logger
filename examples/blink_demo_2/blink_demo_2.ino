#include <Arduino.h>
#include <Wire.h>

int State8 = LOW;
int State9 = LOW;

int delayTime = 1000;   //milliseconds

void setup ()
{
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    Serial.begin(57600);
    delay(1000);
    Serial.println("Mayfly: Blink demo 2");
    delay(1000);
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
    delay(delayTime);
}
