// Force Sensitive Resistor testing sketch. 
 
// Connect the Grove Protoboard to the AA0-AA1 Grove socket on the Mayfly 

// Include, or call, external code Libraries to import extra functionality
#include <Wire.h>               // Call the Wire library for I2C/TWI communication to the ADC chip
#include <Adafruit_ADS1015.h>   // Call the Analog-to-Digital Converter (ADC) chip library

Adafruit_ADS1115 ads;   // Assign Adafruit_ADS1115 functions (16-bit version) to ads  

int LEDpinRed = 8;      // Red LED is connected to Digital pin 5 (PWM pin)
int LEDpinGreen = 9;    // Green LED is connected to Digital pin 6 (PWM pin)
int fsrReading;         // the analog reading from the FSR resistor divider
int LEDbrightness, LEDbrightnessNeg;     //a number between 0 and 255 that controlls the brightness of the LED  


void setup(void) {
  Serial.begin(57600);   // We'll send debugging information via the Serial monitor
  pinMode(LEDpinRed, OUTPUT);       //sets red LED pin as an output
  pinMode(LEDpinGreen, OUTPUT);     //sets green LED pin as an output 
  pinMode(22, OUTPUT);
  digitalWrite(22, HIGH);  
  ads.begin();
}
 
void loop(void) {
  fsrReading = ads.readADC_SingleEnded(0);  //does an analog read sample of the voltage coming from the FSR sensor
  Serial.print("Analog reading = ");        //prints the data to the serial port
  //Serial.println(fsrReading);
  int FSRscaled = map(fsrReading, 0, 16800, 0, 100);
  Serial.println(FSRscaled);
  //digitalWrite(5, HIGH);
  // we'll need to change the range from the analog reading (0-1023) down to the range
  // used by analogWrite (0-255) with map function
  LEDbrightness = map(FSRscaled, 0, 100, 0, 255);
  LEDbrightnessNeg = map(FSRscaled, 0, 100, 255, 0);
  // LED gets brighter the harder you press
  analogWrite(LEDpinRed, LEDbrightness);
  analogWrite(LEDpinGreen, LEDbrightnessNeg); 
  delay(50);       //waits 50ms (0.05 seconds) then starts the loop over again
}
