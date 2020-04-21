/**************************************************************************
battery_measurement.ino

* Written By:  Shannon Hicks <shicks@stroudcenter.org>
* Documentation By: Anthony Aufdenkampe <aaufdenkampe@limno.com>
* Creation Date: 2016
* Development Environment: Arduino IDE 1.6.5+
* Hardware Platform: Stroud Center, EnviroDIY Mayfly Arduino Datalogger
* Software License: BSD-3. 
*   Copyright (c) 2016, Stroud Water Research Center (SWRC) 
*   and the EnviroDIY Development Team

**************************************************************************/


#include <Arduino.h>
#include <Wire.h>       // This library is included with the Arduino IDE, and allows communication with I2C/TWI devices

int batteryPin = A6;    // on the Mayfly board, pin A6 is connected to a resistor divider on the battery input; R1 = 10 Mohm, R2 = 4.7 Mohm

int batterysenseValue = 0;  // variable to store the value coming from the analogRead function
float batteryvoltage;       // the battery voltage as calculated by the formula below

void setup() {
  pinMode(8, OUTPUT);         // declare the Green ledPin as an OUTPUT
  digitalWrite(8, HIGH);      // turn the Green ledPin on
  Serial.begin(57600);
}

void loop() {
  // read the voltage value from the battery input resistor divider, using the 10-bit Analog-to-Digital (ADC) converter in the main Arduino processor (ATmega1284p for Mayfly):
  batterysenseValue = analogRead(batteryPin); // The output from the 10-bit ADC is an integer from 0 to 1023 (2^10 = 1024) that is proportional to the input voltage
  Serial.print("analogValue= ");
  Serial.print(batterysenseValue);
  Serial.print(" bits;    ");

  // Convert ADC output to raw voltage by multiplying by the maximum voltage of the ADC (3.3V), then multiplied by 4.7, which is the resistor divider factor (see ReadMe).
  batteryvoltage = (3.3/1023.) * 4.7 * batterysenseValue;    //for Mayfly v0.5 and later      
  //batteryvoltage = (3.3/1023.) * 1.47 * batterysenseValue;   //for Mayfly v0.3 and v0.4
  
  Serial.print("Voltage: ");
  Serial.print(batteryvoltage);
  Serial.println(" volts");
  delay(1000);

}
