// This Mayfly sketch parses data from Grove Digital Humidity and Temperature (DHT) board
// and prints it on the serial monitor and a SSD1306 OLED display

//Connect the Grove Digital Humidity and Temperature (DHT) board to D10-11 Grove connector
//Connect the OLED display to the Mayfly's I2C Grove connector

// Import required libraries
#include <Arduino.h>
#include <SDL_Arduino_SSD1306.h>    // Modification of Adafruit_SSD1306 for ESP8266 compatibility
#include <AMAdafruit_GFX.h>   // Needs a little change in original Adafruit library (See README.txt file)
#include <SPI.h>            // For SPI comm (needed for not getting compile error)
#include <Wire.h>           // For I2C comm, but needed for not getting compile error
#include <SoftwareSerial.h>
#include "DHT.h"      // Includes the Adafruit DHT-sensor-library 1.3.0+, which was updated to require the Unified Adafruit_Sensor sensor

// Pin definitions
SDL_Arduino_SSD1306 display(4); // FOR I2C

#define DHTPIN 10     // what pin the DHT signal is connected to
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

float h;
float t;

void setup()
{
  Serial.begin(57600);                                      //start serial port for display
//  sonarSerial.begin(9600);                                 //start serial port for maxSonar
  pinMode(5, INPUT);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false);  // initialize with the I2C addr 0x3C (for the 128x64)
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Mayfly");
  display.println("DHT demo");
  display.display();

//DHT stuff
   pinMode(22, OUTPUT);    // Setting up Pin 22 to provide power to Grove Ports
   digitalWrite(22, HIGH); // Provide power to D10-11 and D6-7 Grove Ports
   delay(200);
   Serial.println("Digital Humidity/Temperature");

    dht.begin();

  delay(3000);
}

void loop()
{
  
    // Reading temperature or humidity takes about 250 milliseconds
    // Sensor readings may also be up to 2 seconds old
    float h = dht.readHumidity();
    float t = dht.readTemperature();

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
        Serial.print("Temperature: ");
        Serial.print(t);
        Serial.println(" *C");

        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0,0);
        display.println("Humidity: ");
        display.print(h);
        display.println(" %");
        display.println("Temp: ");
        display.print(t);
        display.println(" *C");
      display.display();
    
        delay(700);
    }
  
  
}



