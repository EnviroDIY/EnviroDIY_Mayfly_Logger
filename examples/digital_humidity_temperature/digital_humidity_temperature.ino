/**************************************************************************************

Logs temperature and humidity at one second intervals to serial monitor.
Written for Mayfly logger and Grove digital temperature/humidity board (DHT11), with 
options for DHT 21 and 22. 
Connect the Grove Digital Temp/Humidity board to D10-11 Grove connector on the Mayfly.
After upload open the serial monitor (magnifying lens button) to view data.

modified 7 Jun 2017 by Beth Fisher github.com/fisherba
**************************************************************************************/


//Arduino sketches begin with a list of the outside libraries needed to run the sketch. 
//They must be in your .../Arduino/libraries directory for the sketch to run.
//#include <Arduino.h>  //Arduino IDE automatically includes Arduino.h, but other IDEs require that you "include" it.
//#include <Wire.h>   // This library is included with the Arduino IDE, and allows communication with I2C/TWI devices.
//Wire.h is required for I2C (Inter-integrated Circuit) and TWI (Two-Wire Interface) connections.
#include "DHT.h"

#define DHTPIN 10     // Specifies the pin connected to the DHT signal.

// Uncomment the digital temperature/humidity model you're using
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
    pinMode(22, OUTPUT);      //pin D22 is the enable line for the Mayfly's switched 3.3/5v power lines
    digitalWrite(22, HIGH);   //set this pin high and leave it on for the rest of the sketch
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
        delay(1000);
    }
}
