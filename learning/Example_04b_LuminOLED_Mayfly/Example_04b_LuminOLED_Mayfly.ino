//Grove digital light sensor test sketch

//Connect the Grove Digital Light board to the I2C Grove socket on the Mayfly

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_TSL2561_U.h>  // Adafruit_TSL2561 library for the TSL2561 digital luminosity (light) sensors
#include <SDL_Arduino_SSD1306.h>    // Modification of Adafruit_SSD1306 for ESP8266 compatibility
#include <AMAdafruit_GFX.h>   // Needs a little change in original Adafruit library (See README.txt file)
#include <SPI.h>            // For SPI comm (needed for not getting compile error)


// Create an instance of the TLS Sensor, using the correct I2C address
// Un-comment the correct one
Adafruit_TSL2561_Unified tsl(TSL2561_ADDR_LOW);  // I2C address 0x29 (addr pin set LOW)
// Adafruit_TSL2561_Unified tsl(TSL2561_ADDR_FLOAT);  // I2C address 0x39 (addr pin left floating)
// Adafruit_TSL2561_Unified tsl(TSL2561_ADDR_HIGH);  // I2C address 0x49 (addr pin set high)

// Create variables for the full spectrum (broadband) and IR luminosity results
uint16_t broadband, ir, visible, lux;

// Create an instance of the OLED display
SDL_Arduino_SSD1306 display(4); // FOR I2C


// The main setup function
void setup()
{
  Serial.begin(57600);

  pinMode(5, INPUT);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false);  // initialize with the I2C addr 0x3C (for the 128x64)
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Mayfly");
  display.println("Lumin demo");
  display.display();

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
  tsl.setGain(TSL2561_GAIN_1X);         // set no gain (for bright situtations)
  //tsl.setGain(TSL2561_GAIN_16X);      // set 16x gain (for dim situations)

  // Changing the integration time gives you a longer time over which to sense light
  // longer timelines are slower, but are good in very low light situtations!
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);  // shortest integration time (bright light)
  //tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  // medium integration time (medium light)
  //tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);  // longest integration time (dim light)

  delay(3000);

  // Now we're ready to get readings!
}


// The loop function, which will run repeatedly
void loop()
{
  // Get both the broadband/full spectrum and IR light intensity from the sensor
  // These values are returned as raw ADC outputs (non-standard units)
  tsl.getLuminosity(&broadband, &ir);
  visible = broadband - ir;
  // Calculate and illuminance in lux (ie, convert sensor units to the standard SI unit)
  lux = tsl.calculateLux(broadband, ir);

  // Print results to the OLED

      if (isnan(broadband))
    {
        Serial.println("Failed to read from Lumin");
        display.println("Lumin failed");
    }
    else
    {
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0,0);
        display.println("Lumin: "); display.print(lux); display.println(" LUX");
        display.display();

        delay(700);
    }


}
