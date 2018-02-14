//Connect the Grove Digital Humidity and Temperature (DHT) board to D10-11 Grove connector on the Mayfly

#include <Arduino.h>
#include <Wire.h>
#include "DHT.h"      // Includes the Adafruit DHT-sensor-library 1.3.0+, which was updated to require the Unified Adafruit_Sensor sensor
#include <Adafruit_TSL2561_U.h>  // Adafruit_TSL2561 library for the TSL2561 digital luminosity (light) sensors


// Create an instance of the TLS Sensor, using the correct I2C address
Adafruit_TSL2561_Unified tsl(TSL2561_ADDR_LOW);  // I2C address 0x29 (addr pin set LOW)

#define DHTPIN 10     // what pin the DHT signal is connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor


// Create variables for the full spectrum (broadband) and IR luminosity results
uint16_t broadband, ir;

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  Serial.begin(57600);

  if (tsl.begin())
  {
    Serial.println("Luminosity sensor");
  }
  else
  {
    Serial.println("No sensor?");
    while (1);
  }

  // You can change the gain on the fly, to adapt to brighter/dimmer light situations
  //tsl.setGain(TSL2561_GAIN_1X);         // set no gain (for bright situtations)
  tsl.setGain(TSL2561_GAIN_16X);      // set 16x gain (for dim situations)

  // Changing the integration time gives you a longer time over which to sense light
  // longer timelines are slower, but are good in very low light situtations!
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);  // shortest integration time (bright light)
  //tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  // medium integration time (medium light)
  //tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);  // longest integration time (dim light)
    
    
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


  // Get both the broadband/full spectrum and IR light intensity from the sensor
  // These values are returned as raw ADC outputs (non-standard units)
  tsl.getLuminosity(&broadband, &ir);
  // Print results to the serial port
  Serial.print("IR: "); Serial.print(ir);   Serial.print("\t\t");
  Serial.print("Full: "); Serial.print(broadband);   Serial.print(" \t");
  Serial.print("Visible: "); Serial.print(broadband - ir);   Serial.print("\t");

  // Calculate and print illuminance in lux (ie, convert sensor units to the standard SI unit)
  Serial.print("Lux: "); Serial.println(tsl.calculateLux(broadband, ir));

        Serial.print("Humidity: ");
        Serial.print(h);
        Serial.print(" %\t");
        Serial.print("Temperature: ");
        Serial.print(t);
        Serial.println(" *C");

  delay(1000);


}
