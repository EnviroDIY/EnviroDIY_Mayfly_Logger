# Reading digital temperature and humidity

This example program:
- Communicates with a digital temperature/humidity board (DHT11)
- Writes the output to the serial port at one second intervals

Written for Mayfly logger and Grove digital temperature/humidity board based on the AOSong DHT (DHT11), with options for DHT 21 and 22.

## Physical connections

- Connect pin 1 (on the left) of the sensor to +5V
- Connect pin 2 of the sensor to whatever your DHTPIN is
- Connect pin 4 (on the right) of the sensor to GROUND
- Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

## Stepping through the code

Arduino sketches (really c++ programs) begin with a list of the outside libraries needed to run the sketch.
These libraries must be installed on your .../Arduino/libraries directory for the sketch to run.
This example only uses two libraries, the Arudino framework library (```Arduino.h```) and the Adafruit library for the AOSong DHT (```DHT.h```).
We use the include that library with the following code:
```cpp
#include <Arduino.h>
#include <DHT.h>
```

After including the libraries, we define exactly which type of sensor we're using and the pin it is connected to.
We then create an "object" in the processor's memory representing the functionality of the DHT sensor.
```cpp
#define DHTPIN 10       // Specifies the pin connected to the DHT signal.
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);
```

The "setup" function runs once when you press reset or power the board.
The "returned value" for this function is "void", which means it doesn't return a value, the function simply performs an action.
In this setup function, we are providing power to the DHT sensor by turning on pin 22.
We are also writing a note to the serial port to appear on the serial monitor.
The last function performs necessary setup for the DHT.
Because we are using a library of functions to communicate with the DHT, we do not need to specify exactly what commands are needed to prepare the DHT.
We call the begin function for the dht object in memory and take advantage of the set-up code provided by that library.
```cpp
void setup()
{
    pinMode(22, OUTPUT);      // pin D22 is the enable line for the Mayfly's switched 3.3/5v power lines
    digitalWrite(22, HIGH);   // set this pin high and leave it on for the rest of the sketch
    delay(200);
    Serial.begin(57600);
    Serial.println("Digital Humidity/Temperature");

    dht.begin();
}
```

The "loop" function runs over and over again.
It performs an action wouthout returning a value, so "void" is once again used in the function declaration.
As in the set-up, we rely on the DHT library to figure out the details of talking to the DHT and call that libraries functions to get the results of the reading.
We print the readings to the serial monitor.
```cpp
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
```