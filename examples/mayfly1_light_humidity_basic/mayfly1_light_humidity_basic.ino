//  A simple sketch demonstrating the two onboard sensors (light and humidity/temp) on the EnviroDIY Mayfly v1.0 board

#include <Wire.h> 
#include "Adafruit_SHT4x.h"

Adafruit_SHT4x sht4 = Adafruit_SHT4x();

void setup () 
{
    Serial.begin(57600);
    Wire.begin();
    Serial.println("EnviroDIY Mayfly v1.0: Light/Humidity/Temp");
    pinMode(8, OUTPUT);
    digitalWrite(8, HIGH);   //turns on the Green LED
    pinMode(9, OUTPUT);   
    digitalWrite(9, HIGH);   //turns on the Red LED

    sht4.begin();
    sht4.setPrecision(SHT4X_HIGH_PRECISION);
    sht4.setHeater(SHT4X_NO_HEATER);
      
}

void loop () 
{
    int light = analogRead(A4);           //reads analog light sensor
    float lightpercent = light/10.23;     //scales light reading as a percentage
    sensors_event_t humidity, temp;
    sht4.getEvent(&humidity, &temp);      // populate temp and humidity objects with fresh data  

    Serial.print("Light: "); Serial.print(lightpercent); Serial.print("%;   ");
    Serial.print("SHT40: ");  Serial.print(temp.temperature); Serial.print(" deg C, ");
    Serial.print(humidity.relative_humidity); Serial.println("% rH");

    delay(2000);
}
