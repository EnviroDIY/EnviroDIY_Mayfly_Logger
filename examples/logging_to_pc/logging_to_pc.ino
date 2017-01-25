/**************************************************************************
logging_to_pc.ino
Written By:  Sara Damiano (sdamiano@stroudcenter.org)
Creation Date: 1/24/2017
Development Environment: PlatformIO 3.2.1
Hardware Platform: Stroud Water Resources Mayfly Arduino Datalogger
Radio Module: None

This sketch is an example of sending data a serial port as if that data
was going to the Web Streaming Data Loader

DISCLAIMER:
THIS CODE IS PROVIDED "AS IS" - NO WARRANTY IS GIVEN.
**************************************************************************/

// -----------------------------------------------
// 1. Include all sensors and necessary files here
// -----------------------------------------------
#include <Wire.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <SPI.h>
#include <SD.h>
#include <RTCTimer.h>
#include <Sodaq_DS3231.h>
#include <Sodaq_PcInt.h>  // Pin Change interrupts, without SDI-12

//#include <SDI12_Mod.h>  // SDI-12 Communications
//#include <Sodaq_PcInt_Mod.h>  // Pin Change interrupts, with SDI-12
//#include <GPRSbee.h>  // GPRSbee Communications
//#include <Adafruit_ADS1015.h>  // Auxillary ADD

// -----------------------------------------------
// 2. Device registration and sampling features
// -----------------------------------------------
const String REGISTRATION_TOKEN = "9e8bbb13-1b71-4720-b930-1d7b0d7602b0";
const String SAMPLING_FEATURE = "a8bff3ec-4ca3-4d13-b1e2-e5effeceb4a1";

// -----------------------------------------------
// 3. WebSDL Endpoints for POST requests
// -----------------------------------------------
const String HOST_ADDRESS = "wpfweb.uwrl.usu.edu";
const String API_ENDPOINT = "/websdl/api/data-stream/";

// -----------------------------------------------
// 4. Misc. Options
// -----------------------------------------------
#define UPDATE_RATE 3000 // milliseconds
#define MAIN_LOOP_DELAY 5000 // milliseconds
#define COMMAND_TIMEOUT 10000 // ms (5000 ms = 5 s)
#define READ_DELAY 1

// -----------------------------------------------
// 5. Board setup info
// -----------------------------------------------
#define SERIAL_BAUD 57600 // Serial port BAUD rate

#define BATTERY_PIN A6  // select the input pin for the potentiometer
#define DATAPIN 7  // change to the proper pin for sdi-12 data pin, pin 7 on shield 3.0
#define SWITCHED_POWER = 22;    // sensor power is pin 22 on Mayfly

#define XB_BAUD 9600  // XBee BAUD rate (9600 is default)
#define GPRSBEE_PWRPIN  23  //DTR
#define XBEECTS_PIN     19   //CTS

#define RTC_PIN A7  // RTC Interrupt pin
#define RTC_INT_PERIOD EveryMinute

#define SD_SS_PIN 12  // SD Card Pin

// -----------------------------------------------
// 6. Global variables
// -----------------------------------------------
unsigned long lastUpdate = 0; // Keep track of last update time
size_t sensorCount = 0;       // Keep this at 0 - it'll get set properly in the setup() function
float ONBOARD_TEMPERATURE = 0;
int currentminute;
long currentepochtime = 0;
int testtimer = 0;
int testminute = 2;
int batterysenseValue = 0;  // variable to store the value coming from the sensor
float batteryvoltage;

enum HTTP_RESPONSE
{
    HTTP_FAILURE = 0,
    HTTP_SUCCESS,
    HTTP_TIMEOUT,
    HTTP_SERVER_ERROR,
    HTTP_REDIRECT,
    HTTP_OTHER
};
Sodaq_DS3231 sodaq;   // Controls the Real Time Clock Chip
RTCTimer timer;  // The timer functions for the RTC
//Adafruit_ADS1115 ads;     // The Auxillary 16-bit ADD chip
//SDI12 mySDI12(DATAPIN);   // The SDI-12 Library

// -----------------------------------------------
// 8. Working functions
// -----------------------------------------------

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
    // Get the temperature from the Mayfly's real time clock and convert to Farenheit
    rtc.convertTemperature();  //convert current temperature into registers
    float tempVal = rtc.getTemperature();
    ONBOARD_TEMPERATURE = (tempVal * 9.0 / 5.0) + 32.0; // Convert to farenheit
    return true;
}

String generateSensorDataString(void)
{
    String jsonString = "{ ";
    jsonString += "\"timestamp\": \"" + getDateTime() + "\", ";
    jsonString += "\"sampling_feature\": \"" + SAMPLING_FEATURE + "\", ";
    jsonString += "\"71408b33-b486-436c-8ed6-7382491f1e12\": " + String(int(ONBOARD_TEMPERATURE));
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
    Serial.begin(SERIAL_BAUD);   // Start the serial connections
    Serial1.begin(XB_BAUD);      // XBee hardware serial connection
    Serial.println("WebSDL Device: EnviroDIY Mayfly\n");
}

void loop()
{
    // Check to see if it is time to post the data to the server
    if (millis() > (lastUpdate + UPDATE_RATE))
    {
        lastUpdate = millis();
        Serial.println("\n---\n---\n");
        updateAllSensors(); // get the sensor value, store as string
        String request = generatePostRequest(generateSensorDataString());
        int result = postData(request);
        printPostResult(result);
    }

    delay(MAIN_LOOP_DELAY);
}
