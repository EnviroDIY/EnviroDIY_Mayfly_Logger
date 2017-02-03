// Force Sensitive Resistor testing sketch.

// Connect the "Mayfly Grove Analog Sensor Board" to the Mayfly's 20-pin Analog header
// Pug the Force Sensitive Resistor into the 2-pin header labeled "FSR"
#include <Arduino.h>
#include <wire.h>


int LEDpinRed = 8;      // Red LED is connected to Digital pin 5 (PWM pin)
int LEDpinGreen = 9;    // Green LED is connected to Digital pin 6 (PWM pin)
int fsrReading;         // the analog reading from the FSR resistor divider
int LEDbrightness, LEDbrightnessNeg;     //a number between 0 and 255 that controlls the brightness of the LED


void setup(void) {
  Serial.begin(57600);   // We'll send debugging information via the Serial monitor
  pinMode(LEDpinRed, OUTPUT);       //sets red LED pin as an output
  pinMode(LEDpinGreen, OUTPUT);     //sets green LED pin as an output
}

void loop(void) {
  fsrReading = analogRead(0);  //does an analog read sample of the voltage coming from the FSR sensor
  Serial.print("Analog reading = ");        //prints the data to the serial port
  int FSRscaled = map(fsrReading, 0, 1024, 0, 100);
  Serial.println(FSRscaled);
  // we'll need to change the range from the analog reading (0-1023) down to the range
  // used by analogWrite (0-255) with map function
  LEDbrightness = map(FSRscaled, 0, 100, 0, 255);
  LEDbrightnessNeg = map(FSRscaled, 0, 100, 255, 0);
  // LED gets brighter the harder you press
  analogWrite(LEDpinRed, LEDbrightness);
  analogWrite(LEDpinGreen, LEDbrightnessNeg);
  delay(50);       //waits 50ms (0.05 seconds) then starts the loop over again
}
