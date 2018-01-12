//Grove digital light sensor test sketch

//Connect the Grove Digital Light board to the I2C Grove socket on the Mayfly

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_TSL2561_U.h>  // Adafruit_TSL2561 library for the TSL2561 digital luminosity (light) sensors


// Create an instance of the TLS Sensor, using the correct I2C address
// Un-comment the correct one
Adafruit_TSL2561_Unified tsl(TSL2561_ADDR_LOW);  // I2C address 0x29 (addr pin set LOW)
// Adafruit_TSL2561_Unified tsl(TSL2561_ADDR_FLOAT);  // I2C address 0x39 (addr pin left floating)
// Adafruit_TSL2561_Unified tsl(TSL2561_ADDR_HIGH);  // I2C address 0x49 (addr pin set high)

// Create variables for the full spectrum (broadband) and IR luminosity results
uint16_t broadband, ir;


// The main setup function
void setup(void) {
  Serial.begin(57600);

  if (tsl.begin()) {
    Serial.println("Luminosity sensor");
  }
  else
  {
    Serial.println("No sensor?");
    while (1);
  }

  // You can change the gain on the fly, to adapt to brighter/dimmer light situations
  //tsl.setGain(TSL2561_GAIN_0X);         // set no gain (for bright situtations)
  tsl.setGain(TSL2561_GAIN_16X);      // set 16x gain (for dim situations)

  // Changing the integration time gives you a longer time over which to sense light
  // longer timelines are slower, but are good in very low light situtations!
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);  // shortest integration time (bright light)
  //tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  // medium integration time (medium light)
  //tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);  // longest integration time (dim light)

  // Now we're ready to get readings!
}


// The loop function, which will run repeatedly
void loop(void) {

  // Get both the broadband/full spectrum and IR light intensity from the sensor
  // These values are returned as raw ADC outputs (non-standard units)
  tsl.getLuminosity(&broadband, &ir);
  // Print results to the serial port
  Serial.print("IR: "); Serial.print(ir);   Serial.print("\t\t");
  Serial.print("Full: "); Serial.print(broadband);   Serial.print(" \t");
  Serial.print("Visible: "); Serial.print(broadband - ir);   Serial.print("\t");

  // Calculate and print illuminance in lux (ie, convert sensor units to the standard SI unit)
  Serial.print("Lux: "); Serial.println(tsl.calculateLux(broadband, ir));

  delay(100);
}
