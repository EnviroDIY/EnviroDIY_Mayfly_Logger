#include <Arduino.h>

int stateGreenLED = LOW;
int pinGreenLED = 8;
int stateRedLED = LOW;
int pinRedLED = 9;

int delayTime = 1000; // milliseconds

void setup()
{
    // set each pin as output to send signals out to the LED
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    // establish the data rate for serial transmission
    Serial.begin(57600);
    delay(1000);
    // send a message to the Serial Monitor
    Serial.println("Mayfly: Blink demo 2");
    delay(1000);
}

void loop()
{
    // Check the previous value of the green LED
    // and re-assign it to the opposite of what it had been.
    if (stateGreenLED == LOW)
    {
        stateGreenLED = HIGH;
    }
    else
    {
        stateGreenLED = LOW;
    }
    digitalWrite(pinGreenLED, stateGreenLED);
    stateRedLED = !stateGreenLED;
    // Set the red LED to the oposite of whatever the green is
    digitalWrite(pinRedLED, stateRedLED);
    delay(delayTime);
}
