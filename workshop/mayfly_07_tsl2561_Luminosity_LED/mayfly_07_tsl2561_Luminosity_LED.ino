//Grove digital light sensor test sketch

//Connect the Grove Digital Light board to the I2C Grove socket on the Mayfly

#include <Arduino.h>
#include <Wire.h>
#include "TSL2561.h"  // Adafruit_TSL2561 library for the TSL2561 digital luminosity (light) sensors
#include <SDL_Arduino_SSD1306.h>    // Modification of Adafruit_SSD1306 for ESP8266 compatibility
#include <AMAdafruit_GFX.h>   // Needs a little change in original Adafruit library (See README.txt file)
#include <SPI.h>            // For SPI comm (needed for not getting compile error)


TSL2561 tsl(TSL2561_ADDR_LOW);
SDL_Arduino_SSD1306 display(4); // FOR I2C

uint16_t x;

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
  tsl.setGain(TSL2561_GAIN_0X);         // set no gain (for bright situtations)
  //tsl.setGain(TSL2561_GAIN_16X);      // set 16x gain (for dim situations)

  // Changing the integration time gives you a longer time over which to sense light
  // longer timelines are slower, but are good in very low light situtations!
  tsl.setTiming(TSL2561_INTEGRATIONTIME_13MS);  // shortest integration time (bright light)
  //tsl.setTiming(TSL2561_INTEGRATIONTIME_101MS);  // medium integration time (medium light)
  //tsl.setTiming(TSL2561_INTEGRATIONTIME_402MS);  // longest integration time (dim light)

  delay(3000);

  // Now we're ready to get readings!
}

void loop() 
{
  // Simple data read example. Just read the infrared, fullspecrtrum diode
  // or 'visible' (difference between the two) channels.
  // This can take 13-402 milliseconds! Uncomment whichever of the following you want to read
  uint16_t x = tsl.getLuminosity(TSL2561_VISIBLE);
  //uint16_t x = tsl.getLuminosity(TSL2561_FULLSPECTRUM);
  //uint16_t x = tsl.getLuminosity(TSL2561_INFRARED);

  Serial.println(x, DEC);   // print simple visible

  // More advanced data read example. Read 32 bits with top 16 bits IR, bottom 16 bits full spectrum
  // That way you can do whatever math and comparions you want!
  //uint32_t lum = tsl.getFullLuminosity();
  //uint16_t ir, full;
  //ir = lum >> 16;
  //full = lum & 0xFFFF;
  //Serial.print("IR: "); Serial.print(ir);   Serial.print("\t\t");
  //Serial.print("Full: "); Serial.print(full);   Serial.print(" \t");
  //Serial.print("Visible: "); Serial.print(full - ir);   Serial.print("\t");

  //Serial.print("Lux: "); Serial.println(tsl.calculateLux(full, ir));

      if (isnan(x))
    {
        Serial.println("Failed to read from Lumin");
        display.println("Lumin failed");
    }
    else
    {
        
        Serial.print("Lumin: "); Serial.print(x); Serial.print(" LUX");
        
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0,0);
        display.println("Lumin: "); display.print(x); display.println(" LUX");
        display.display();
    
        delay(700);
    }
  
  
}
