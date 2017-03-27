// This Mayfly sketch parses data from MaxSonar serial data and prints it on the serial monitor and a SSD1306 OLED display

// To use this sketch, connect the MaxBotix serial data output pin (pin 5) to Mayfly pin D5.
// Connect the MaxBotix power pin (pin 6) to the Vcc pin next to Mayfly pin D5.
// Connect the MaxBotix power pin (pin 7) to the ground pin near Mayfly pin D5.
// Set the jumper controlling power to pins D4-5 to be continuously powered.
// Leave all other pins on the MaxBotix unconnected
// Connect the OLED display to the Mayfly's I2C Grove connector

// Import required libraries
#include <Arduino.h>
#include <SDL_Arduino_SSD1306.h>    // Modification of Adafruit_SSD1306 for ESP8266 compatibility
#include <AMAdafruit_GFX.h>   // Needs a little change in original Adafruit library (See README.txt file)
#include <SPI.h>            // For SPI comm (needed for not getting compile error)
#include <Wire.h>           // For I2C comm, but needed for not getting compile error
#include <SoftwareSerial.h>

// Pin definitions
SDL_Arduino_SSD1306 display(4); // FOR I2C
SoftwareSerial sonarSerial(5, -1);            //define serial port for recieving data.


int parseSonar(void)
{
    bool stringComplete = false;
    int rangeAttempts = 0;
    int result = 0;

    // Serial.println(F("Beginning detection for Sonar"));  // For debugging
    while (stringComplete == false && rangeAttempts < 50)
    {
        result = sonarSerial.parseInt();
        sonarSerial.read();  // To throw away the carriage return

        Serial.print("Range: ");
        Serial.print(result);
        Serial.println(" mm");

        rangeAttempts++;

        // If it cannot obtain a result , the sonar is supposed to send a value
        // just above it's max range.  For 10m models, this is 9999, for 5m models
        // it's 4999.  The sonar might also send readings of 300 or 500 (the
        //  blanking distance) if there are too many acoustic echos.
        // If the result becomes garbled or the sonar is disconnected, the parseInt function returns 0.
        if (result == 0 || result == 300 || result == 500 || result == 4999 || result == 9999)
        {
            Serial.print(F("Bad or Suspicious Result, Retry Attempt #"));  // For debugging
            Serial.println(rangeAttempts);  // For debugging
        }
        else
        {
            Serial.println(F("Good result found"));  // For debugging
            stringComplete = true;  // Set completion of read to true
        }
    }
    return result;
}

void setup()
{
    Serial.begin(57600);  // Start serial port for display

    pinMode(5, INPUT);  // Set the pin mode for the software serial port
    sonarSerial.begin(9600);  // Start serial port for maxSonar
    sonarSerial.setTimeout(180);  // Set a timeout for the serial instance
    // Even the slowest sensors should respond at a rate of 6Hz (166ms).

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false);  // initialize with the I2C addr 0x3C (for the 128x64)
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("Mayfly");
    display.println("Sonar demo");
    display.display();

    Serial.println("Mayfly MaxBotix sonar sensor rangefinder example");
}

void loop()
{
    int range = parseSonar();

    display.clearDisplay();
    display.setTextSize(3);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("Range:");
    display.print(range);
    display.println(" mm");
    display.display();

    delay(1000);
}
