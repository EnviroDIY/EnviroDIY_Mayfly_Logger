/**************************************************************************
Mayfly_BeeWiFi_USU.ino
Written By:  Jeff Horsburgh (jeff.horsburgh@usu.edu)
Updated By:  Kenny Fryar-Ludwig (kenny.fryarludwig@usu.edu)
Creation Date: 6/3/2016
Updated: 1/24/2017
Development Environment: Arduino 1.6.9
Hardware Platform: Stroud Water Resources Mayfly Arduino Datalogger
Radio Module: Bee S6b WiFi module.

This sketch is an example of posting data to the EnviroDIY Water Quality
data portal (http://data.envirodiy.org) using a Mayfly Arduino board and an
Bee Wifi module. As a quick example, it uses the temperature values from
the Mayfly's real time clock and POSTs them to http://data.envirodiy.org.
This sketch could easily be modified to post any sensor measurements to a stream
at http://data.envirodiy.org that has been configured to accept them.

This sketch was adapted from Jim Lindblom's example at:

https://learn.sparkfun.com/tutorials/internet-datalogging-with-arduino-and-xbee-wifi

Assumptions:
1. The Bee WiFi module has must be configured correctly to connect to the
wireless network prior to running this sketch.
2. The Mayfly has been registered at http://data.envirodiy.org and the sensor
has been configured. In this example, only temperature is used.

DISCLAIMER:
THIS CODE IS PROVIDED "AS IS" - NO WARRANTY IS GIVEN.
**************************************************************************/


// -----------------------------------------------
// Note: All 'Serial.print' statements can be
// removed if they are not desired - used for
// debugging only
// -----------------------------------------------


// -----------------------------------------------
// 1. Include all sensors and necessary files here
// -----------------------------------------------
#include <Wire.h>
#include "Sodaq_DS3231.h"

// -----------------------------------------------
// 2. Device registration and sampling features
// -----------------------------------------------
// Register your site and get these tokens from data.envirodiy.org
const String REGISTRATION_TOKEN = "94482d7f-0a2a-41af-b483-d3fdb1da3c53";
const String SAMPLING_FEATURE = "1fee64b3-4ba7-43ac-925f-1d199f37f962";

// -----------------------------------------------
// 3. WebSDL Endpoints for POST requests
// -----------------------------------------------
const String HOST_ADDRESS = "data.envirodiy.org";
const String API_ENDPOINT = "/api/data-stream/";

// -----------------------------------------------
// 4. Misc. Options
// -----------------------------------------------
#define UPDATE_RATE 30000 // milliseconds - 30000 = 30 seconds
#define MAIN_LOOP_DELAY 5000 // milliseconds
#define COMMAND_TIMEOUT 10000 // ms (5000 ms = 5 s)

// -----------------------------------------------
// 5. Board setup info
// -----------------------------------------------
#define XB_BAUD 9600 // Bee BAUD rate (9600 is default)
#define SERIAL_BAUD 57600 // Serial port BAUD rate

// -----------------------------------------------
// 6. Global variables
// -----------------------------------------------
unsigned long lastUpdate = 0; // Keep track of last update time
Sodaq_DS3231 sodaq;           // This is used for some board functions
size_t sensorCount = 0;       // Keep this at 0 - it'll get set properly in the setup() function
float ONBOARD_TEMPERATURE = 0;

enum HTTP_RESPONSE
{
    HTTP_FAILURE = 0,
    HTTP_SUCCESS,
    HTTP_TIMEOUT,
    HTTP_SERVER_ERROR,
    HTTP_REDIRECT,
    HTTP_OTHER
};

// Used to flush out the buffer after a post request.
// Removing this may cause communication issues. If you
// prefer to not see the std::out, remove the print statement
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

// Used only for debugging - can be removed
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
            Serial.println("\nAn unknown error has occured, and we're pretty confused\n");
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
    if (timeout > 0 && Serial1.available() >= 12)
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

// This function generates the POST request that gets sent to data.envirodiy.org
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

// This function updates the values for any connected sensors. Need to add code for
// Any sensors connected - this example only uses temperature.
bool updateAllSensors()
{
    // Get the temperature from the Mayfly's real time clock and convert to Farenheit
    rtc.convertTemperature();  //convert current temperature into registers
    float tempVal = rtc.getTemperature();
    ONBOARD_TEMPERATURE = (tempVal * 9.0 / 5.0) + 32.0; // Convert to farenheit
    return true;
}

// This function returns the datetime from the realtime clock
String getDateTime(void)
{
    DateTime currDateTime = sodaq.now();
    String date = String(currDateTime.year()) + "-" + String(currDateTime.month()) + "-" + String(currDateTime.date()) + " ";
    String time = String(currDateTime.hour()) + ":" + String(currDateTime.minute()) + ":" + String(currDateTime.second());
    return date + time;
}

// This function generates the JSON data string that becomes the body of the POST request
// For now, the Result UUID is hard coded here
// TODO:  Move the Result UUID somewhere easier to configure.
String generateSensorDataJSON(void)
{
    String jsonString = "{ ";
    jsonString += "\"timestamp\": \"" + getDateTime() + "\", ";
    jsonString += "\"sampling_feature\": \"" + SAMPLING_FEATURE + "\", ";
    jsonString += "\"ec0ad1f4-17bf-4ee5-a56d-3d5911e80825\": " + String(int(ONBOARD_TEMPERATURE));
    jsonString += " }";
    return jsonString;
}

// Main setup function
void setup()
{
    sodaq.setEpoch(1481224540);  // Use this to set the current time, set to current unix epoch
    Serial.begin(SERIAL_BAUD);   // Start the serial connections
    Serial1.begin(XB_BAUD);      // Bee hardware serial connection
    Serial.println("WebSDL Device: EnviroDIY Mayfly\n");
}

void loop()
{
    // Check to see if it is time to post the data to the server
    if (millis() > (lastUpdate + UPDATE_RATE))
    {
        lastUpdate = millis();
        Serial.println("\n---\n---\n");
        updateAllSensors(); // get the sensor value(s), store as string
        String request = generatePostRequest(generateSensorDataJSON());
        int result = postData(request);
        printPostResult(result);
    }

    delay(MAIN_LOOP_DELAY);
}
