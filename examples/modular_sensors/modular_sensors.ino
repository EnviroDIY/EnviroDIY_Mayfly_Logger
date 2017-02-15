#include <Arduino.h>

/**************************************************************************
Mayfly_XBeeWiFi.ino
Written By:  Jeff Horsburgh (jeff.horsburgh@usu.edu)
Updated By:  Kenny Fryar-Ludwig (kenny.fryarludwig@usu.edu)
Creation Date: 6/3/2016
Development Environment: Arduino 1.6.9
Hardware Platform: Stroud Water Resources Mayfly Arduino Datalogger
Radio Module: XBee S6b WiFi module.

This sketch is an example of posting data to SparkFun's data stream server
(http://data.sparkfun.com) using a Mayfly Arduino board and an XBee Wifi
module. As a quick example, it uses the temperature values from the Mayfly's
real time clock and POSTs them to a stream at http://data.sparkfun.com. This
sketch could easily be modified to post any sensor measurements to a stream
at data.sparkfun.com that has been configured to accept them.

This sketch was adapted from Jim Lindblom's example at:

https://learn.sparkfun.com/tutorials/internet-datalogging-with-arduino-and-xbee-wifi

Assumptions:
1. The XBee WiFi module has must be configured correctly to connect to the
wireless network prior to running this sketch.

DISCLAIMER:
THIS CODE IS PROVIDED "AS IS" - NO WARRANTY IS GIVEN.
**************************************************************************/
#include <Wire.h>
#include "Config.h"
#include "Sodaq_DS3231.h"

unsigned long lastUpdate = 0; // Keep track of last update time
Sodaq_DS3231 sodaq;           // This is used for some board functions
size_t sensorCount = 0;       // Keep this at 0 - it'll get set properly in the setup() function

enum HTTP_RESPONSE
{
    HTTP_FAILURE = 0,
    HTTP_SUCCESS,
    HTTP_TIMEOUT,
    HTTP_SERVER_ERROR,
    HTTP_REDIRECT,
    HTTP_OTHER
};

void printRemainingChars(int timeDelay = 1, int timeout = 5000)
{
    while (timeout-- > 0 && Serial1.available() > 0)
    {
        while (Serial1.available() > 0)
        {
            char netChar = Serial1.read();
            Serial.print(netChar);
            delay(timeDelay);
        }

        delay(timeDelay);
    }

    Serial1.flush();
}

void printPostResult(int result)
{
    switch (result)
    {
        case HTTP_SUCCESS:
        {
            Serial.println("\nSucessfully sent data to " + HOST_ADDRESS + "\n");
        }
        break;

        case HTTP_FAILURE:
        {
            Serial.println("\nFailed to send data to " + HOST_ADDRESS + "\n");
        }
        break;

        case HTTP_TIMEOUT:
        {
            Serial.println("\nRequest to " + HOST_ADDRESS + " timed out, no response from server.\n");
        }
        break;

        case HTTP_REDIRECT:
        {
            Serial.println("\nRequest to " + HOST_ADDRESS + " was redirected.\n");
        }
        break;

        case HTTP_SERVER_ERROR:
        {
            Serial.println("\nRequest to " + HOST_ADDRESS + " caused an internal server error.\n");
        }
        break;

        default:
        {
            Serial.println("\An unknown error has occured, and we're pretty confused\n");
        }
    }
}

// This function makes an HTTP connection to the server and POSTs data
int postData(String requestString, bool redirected = false)
{
    Serial.println("Checking for remaining data in the buffer");
    printRemainingChars(5, 5000);
    Serial.println("\n");

    HTTP_RESPONSE result = HTTP_OTHER;

    Serial1.flush();
    Serial1.print(requestString.c_str());
    Serial1.flush();


    Serial.flush();
    Serial.println(" -- Request -- ");
    Serial.print(requestString.c_str());
    Serial.flush();

    // Add a brief delay for at least the first 12 characters of the HTTP response
    int timeout = COMMAND_TIMEOUT;
    while ((timeout-- > 0) && Serial1.available() > 0)
    {
        delay(2);
    }

    // Process the HTTP response
    if (timeout > 0 || Serial1.available() >= 12)
    {
        char response[10];
        char code[4];
        memset(response, '\0', 10);
        memset(code, '\0', 4);

        int responseBytes = Serial1.readBytes(response, 9);
        int codeBytes = Serial1.readBytes(code, 3);
        Serial.println("\n -- Response -- ");
        Serial.print(response);
        Serial.println(code);

        printRemainingChars(5, 5000);

        // Check the response to see if it was successful
        if (memcmp(response, "HTTP/1.0 ", responseBytes) == 0
            || memcmp(response, "HTTP/1.1 ", responseBytes) == 0)
        {
            if (memcmp(code, "200", codeBytes) == 0
                || memcmp(code, "201", codeBytes) == 0)
            {
                // The first 12 characters of the response indicate "HTTP/1.1 200" which is success
                result = HTTP_SUCCESS;
            }
            else if (memcmp(code, "302", codeBytes) == 0)
            {
                result = HTTP_REDIRECT;
            }
            else if (memcmp(code, "400", codeBytes) == 0
                || memcmp(code, "404", codeBytes) == 0)
            {
                result = HTTP_FAILURE;
            }
            else if (memcmp(code, "500", codeBytes) == 0)
            {
                result = HTTP_SERVER_ERROR;
            }
        }
    }
    else // Otherwise timeout, no response from server
    {
        result = HTTP_TIMEOUT;
    }

    return result;
}

String generatePostRequest(String dataString)
{
    String request = "POST " + API_ENDPOINT + " HTTP/1.1\r\n";
    request += "Host: " + HOST_ADDRESS + "\r\n";
    request += "token: " + REGISTRATION_TOKEN + "\r\n";
    request += "Content-Type: application/json\r\n";
    request += "Cache-Control: no-cache\r\n";
    request += "Content-Length: " + String(dataString.length() + 3) + "\r\n";
    request += "\r\n";
    request += dataString;
    request += "\r\n\r\n";

    return request;
}

bool updateAllSensors()
{
    bool success = true;

    for (int i = 0; i < sensorCount; i++)
    {
        success &= SENSOR_LIST[i]->update();
    }

    return success;
}

String generateSensorDataString(void)
{
    String jsonString = "{ ";
    jsonString += "\"timestamp\": \"" + getDateTime() + "\", ";
    jsonString += "\"sampling_feature\": \"" + SAMPLING_FEATURE + "\", ";

    for (int i = 0; i < sensorCount; i++)
    {
        jsonString += "\"" + SENSOR_LIST[i]->getName() + "\": " + SENSOR_LIST[i]->getValueAsString();
        if (i + 1 != sensorCount)
        {
            jsonString += ", ";
        }
    }

    jsonString += " }";
    return jsonString;
}

String getDateTime(void)
{
    DateTime currDateTime = sodaq.now();
    String date = String(currDateTime.year()) + "-" + String(currDateTime.month()) + "-" + String(currDateTime.date()) + " ";
    String time = String(currDateTime.hour()) + ":" + String(currDateTime.minute()) + ":" + String(currDateTime.second());
    return date + time;
}

void setup()
{
    sodaq.setEpoch(1476912806);  // Use this to set the current time, set to current unix epoch
    Serial.begin(SERIAL_BAUD);   // Start the serial connections
    Serial1.begin(XB_BAUD);      // XBee hardware serial connection
    sensorCount = sizeof(SENSOR_LIST) / sizeof(SENSOR_LIST[0]);
    Serial.println("There are " + String(sensorCount) + " sensors");
    Serial.println("WebSDL Device: EnviroDIY Mayfly\n");
}

void loop()
{
    // Check to see if it is time to post the data to the server
    if (millis() > (lastUpdate + UPDATE_RATE))
    {
        lastUpdate = millis();

        Serial.println("\n----------\n----------\n");
        updateAllSensors(); // get the sensor value, store as string
/*
        String request = generatePostRequest(generateSensorDataString());
        int result = postData(request);
        printPostResult(result);*/
    }

    //delay(MAIN_LOOP_DELAY);
}
