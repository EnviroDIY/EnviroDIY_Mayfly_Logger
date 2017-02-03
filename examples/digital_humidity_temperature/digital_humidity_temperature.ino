//This Mayfly sketch reads data from a Grove digital temperature/humidity board (DHT11)

//Connect the Grove Digital Temp/Humidity board to D10-11 Grove connector on the Mayfly

#include <Arduino.h>
#include <wire.h>
#include "DHT.h"

#define DHTPIN 10     // what pin the DHT signal is connected to

// Uncomment whatever type you're using
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
    }
}
