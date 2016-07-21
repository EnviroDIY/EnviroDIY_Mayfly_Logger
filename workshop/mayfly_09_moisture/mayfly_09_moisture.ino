/* Grove Soil Moisture testing sketch

Connect the Grove Protoboard to the AA2-AA3 Grove socket on the Mayfly
 
 */

#include <Wire.h>
#include <Adafruit_ADS1015.h>
 
Adafruit_ADS1115 ads;

int LEDpinRed = 8;      // Red LED 
int LEDpinGreen = 9;    // Green LED
int adcReading;         // the analog reading from the FSR resistor divider
int LEDbrightness, LEDbrightnessNeg;     //a number between 0 and 255 that controls the brightness of the LED  


void setup(void) {
  Serial.begin(57600);   // We'll send debugging information via the Serial monitor
  pinMode(LEDpinRed, OUTPUT);       //sets red LED pin as an output
  pinMode(LEDpinGreen, OUTPUT);     //sets green LED pin as an output 
  pinMode(22, OUTPUT);
  digitalWrite(22, HIGH);  
  Serial.println("Soil Moisture Grove Sensor");
  ads.begin();
}
 
void loop(void) {
  adcReading = ads.readADC_SingleEnded(2);    //does an analog read sample of the voltage coming from the FSR sensor
  Serial.print("Analog reading = ");        //prints the data to the serial port
  //Serial.println(fsrReading);
  int adcscaled = map(adcReading, 0, 15000, 0, 100);
  Serial.println(adcscaled);
  //digitalWrite(5, HIGH);
  // we'll need to change the range from the analog reading (0-1023) down to the range
  // used by analogWrite (0-255) with map function
  LEDbrightness = map(adcscaled, 0, 100, 0, 255);
  LEDbrightnessNeg = map(adcscaled, 0, 100, 255, 0);
  // LED gets brighter the harder you press
  analogWrite(LEDpinRed, LEDbrightness);
  analogWrite(LEDpinGreen, LEDbrightnessNeg); 
  delay(50);       //waits 100ms (0.1 second) then starts the loop over again
}
