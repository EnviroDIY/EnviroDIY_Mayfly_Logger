//These are the outside libraries to include in the sketch. 
//They must be in your Arduino/libraries directory for the sketch to run.
//Strictly speaking neither of these are required for the blink demo.
#include <Arduino.h>
#include <Wire.h>

//This list located before the "void" sets defines the variables for the sketch. 
//This is useful for assigning pins or delay time. For example, Mayfly has LEDs at
//pins 8 and 9.
int delayTime = 500;   //milliseconds

void setup ()
{
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    Serial.begin(57600);
    delay(1000);
    Serial.println("Mayfly: Blink demo 1");
    delay(1000);
}

void loop ()
{
    digitalWrite(8, HIGH);
    digitalWrite(9, HIGH);

    delay(delayTime);

    digitalWrite(8, LOW);
    digitalWrite(9, LOW);

    delay(delayTime);
}
