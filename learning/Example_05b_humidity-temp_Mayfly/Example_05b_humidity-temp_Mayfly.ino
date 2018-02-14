//Connect the Grove Digital Humidity and Temperature (DHT) board to D10-11 Grove connector on the Mayfly

#include <Arduino.h>
#include <Wire.h>
#include "DHT.h"      // Includes the Adafruit DHT-sensor-library 1.3.0+, which was updated to require the Unified Adafruit_Sensor sensor

#define DHTPIN 10     // what pin the DHT signal is connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
    pinMode(22, OUTPUT);    // Setting up Pin 22 to provide power to Grove Ports
    digitalWrite(22, HIGH); // Provide power to D10-11 and D6-7 Grove Ports
    delay(200);
    Serial.begin(57600);
    Serial.println("Digital Humidity/Temperature");

    dht.begin();
}

void loop()
{
    // Reading temperature or humidity takes about 250 milliseconds
    // Sensor readings may also be up to 2 seconds old
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    // now convert to Fahrenheight
    float temperatureF = (t * 9.0 / 5.0) + 32.0;

    // check if returns are valid, if they are NaN (not a number) then something went wrong
    if (isnan(t) || isnan(h))
    {
        Serial.println("Failed to read from DHT");
    }
    else
    {
        Serial.print("Humidity: ");
        Serial.print(h);
        Serial.print(" %\t");
        Serial.print("Temp: ");
        Serial.print(t);
        Serial.print(" *C");
        Serial.print("       Temp: ");
        Serial.print(temperatureF); 
        Serial.println(" *F");
        delay(1000);
    }

 

}
