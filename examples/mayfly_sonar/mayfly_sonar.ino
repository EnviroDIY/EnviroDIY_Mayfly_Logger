// This Mayfly sketch parses data from MaxSonar serial data and prints it on the serial monitor

// To use this sketch, connect the MaxBotix serial data output pin (pin 5) to Mayfly pin D5.
// Connect the MaxBotix power pin (pin 6) to the Vcc pin next to Mayfly pin D5.
// Connect the MaxBotix power pin (pin 7) to the ground pin near Mayfly pin D5.
// Set the jumper controlling power to pins D4-5 to be continuously powered.
// Leave all other pins on the MaxBotix unconnected

#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial sonarSerial(5, -1);  // Define serial port for recieving data.


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

    Serial.println("Mayfly MaxBotix sonar sensor rangefinder example");

    // Read and print the header from the MaxBotix
    // Serial.println(F("----------------------------------------------------"));
    // for(int i=0; i < 6; i++)  // For debugging
    // {
    //     Serial.println(sonarSerial.readStringUntil('\r'));
    // }
    // Serial.println(F("----------------------------------------------------"));

}

void loop()
{
    parseSonar();
    delay(1000);
    Serial.println(F("----------------------------------------------------"));
}
