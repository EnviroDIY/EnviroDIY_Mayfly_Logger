/**************************************************************************
simple_logging_example.ino

Initial Creation Date: 6/3/2016
Written By:  Jeff Horsburgh (jeff.horsburgh@usu.edu)
Updated By:  Kenny Fryar-Ludwig (kenny.fryarludwig@usu.edu)
Additional Work By:  Sara Damiano (sdamiano@stroudcenter.org)
Development Environment: PlatformIO 3.2.1
Hardware Platform: EnviroDIY Mayfly Arduino Datalogger
Radio Module: Sodaq GPRSbee or Digi XBee S6B (WiFi)
Software License: BSD-3.
  Copyright (c) 2017, Stroud Water Research Center (SWRC)
  and the EnviroDIY Development Team

This sketch is an example of posting data to the Web Streaming Data Loader

Assumptions:
1. The Bee WiFi module has must be configured correctly to connect to the
   wireless network prior to running this sketch. (If Applicable)
2. The Mayfly has been registered at http://data.envirodiy.org and the sensor
   has been configured. In this example, only temperature and battery voltage are used.

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
#include <Arduino.h>
#include <Wire.h>
#include <avr/sleep.h>
#include <SdFat.h>
#include <RTCTimer.h>
#include <Sodaq_DS3231.h>
#include <Sodaq_PcInt_PCINT0.h>
#include <GPRSbee.h>

// -----------------------------------------------
// 2. Device registration and sampling features
// -----------------------------------------------
// Skecth file name
const char *SKETCH_NAME = "logging_to_EnviroDIY.ino";

// Data header, for data log file on SD card
const char *LOGGERNAME = "Mayfly 160073";
const char *FILE_NAME = "MF160073";
const char *DATA_HEADER = "JSON Formatted Data";

// Register your site and get these tokens from data.envirodiy.org
const char *REGISTRATION_TOKEN = "3522beb5-f508-458f-b3f2-f37b00068bc7";
const char *SAMPLING_FEATURE = "5bdba7fd-d356-410b-b75f-a44c2a4eea77";
const int TIME_ZONE = -5;
const char *ONBOARD_TEMPERATURE_UUID = "ad6b2392-1e32-4f95-b8f7-394f98957b8a";
const char *ONBOARD_BATTERY_UUID = "443f94f4-9e1e-4068-bdc6-ba0496bfbcbe";

// -----------------------------------------------
// 3. Device Connection Options
// -----------------------------------------------
const char *BEE_TYPE = "WIFI";  // The type of XBee, either "GPRS" or "WIFI"
const char* APN = "apn.konekt.io";  // The APN for the GPRSBee

// -----------------------------------------------
// 4. Timing Options For Logging
// -----------------------------------------------
int LOGGING_INTERVAL = 1;  // How frequently (in minutes) to log data
int READ_DELAY = 1;  // How often (in minutes) the timer wakes up
int UPDATE_RATE = 200; // How frequently (in milliseconds) the logger checks if it should log
int COMMAND_TIMEOUT = 15000;  // How long (in milliseconds) to wait for a server response

// -----------------------------------------------
// 5. WebSDL Endpoints for POST requests
// -----------------------------------------------
const char *HOST_ADDRESS = "data.envirodiy.org";
const char *API_ENDPOINT = "/api/data-stream/";

// -----------------------------------------------
// 6. Board setup info
// -----------------------------------------------
int SERIAL_BAUD = 9600;  // Serial port BAUD rate
int BEE_BAUD = 9600;  // Bee BAUD rate (9600 is default)
int BEE_DTR_PIN = 23;  // Bee DTR Pin (Data Terminal Ready - used for sleep)
int BEE_CTS_PIN = 19;   // Bee CTS Pin (Clear to Send)
int GREEN_LED = 8;  // Pin for the green LED
int RED_LED = 9;  // Pin for the red LED

int RTC_PIN = A7;  // RTC Interrupt pin
#define RTC_INT_PERIOD EveryMinute  //The interrupt period on the RTC

int BATTERY_PIN = A6;    // select the input pin for the potentiometer

int SD_SS_PIN = 12;  // SD Card Pin

// -----------------------------------------------
// 7. Setup variables
// -----------------------------------------------
float ONBOARD_TEMPERATURE = 0;  // Variable to store the temperature result in
float ONBOARD_BATTERY = 0;  // variable to store the value coming from the sensor

// Variables for the timer function
int currentminute;
int testtimer = 0;
int testminute = 1;
long currentepochtime = 0;

enum HTTP_RESPONSE
{
    HTTP_FAILURE = 0,
    HTTP_SUCCESS,
    HTTP_TIMEOUT,
    HTTP_FORBIDDEN,
    HTTP_SERVER_ERROR,
    HTTP_REDIRECT,
    HTTP_OTHER
};
Sodaq_DS3231 sodaq;   // Controls the Real Time Clock Chip
RTCTimer timer;  // The timer functions for the RTC
SdFat SD;  // The SD initialization
String fileName = String(FILE_NAME);  // For the file name
//Adafruit_ADS1115 ads;     // The Auxillary 16-bit ADD chip
//SDI12 mySDI12(DATAPIN_SDI12);   // The SDI-12 Library


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
            // Serial1.read();
            delay(timeDelay);
        }
        delay(timeDelay);
    }
}

// Helper function to get the current date/time from the RTC
// as a unix timestamp - and apply the correct time zone.
uint32_t getNow()
{
  currentepochtime = rtc.now().getEpoch();
  currentepochtime += TIME_ZONE*3600;
  return currentepochtime;
}

// This function returns the datetime from the realtime clock as an ISO 8601 formated string
String getDateTime_ISO8601(void)
{
  String dateTimeStr;
  //Create a DateTime object from the current time
  DateTime dt(rtc.makeDateTime(getNow()));
  //Convert it to a String
  dt.addToString(dateTimeStr);
  dateTimeStr.replace(F(" "), F("T"));
  String tzString = String(TIME_ZONE);
  if (-24 <= TIME_ZONE && TIME_ZONE <= -10)
  {
      tzString += F(":00");
  }
  else if (-10 < TIME_ZONE && TIME_ZONE < 0)
  {
      tzString = tzString.substring(0,1) + F("0") + tzString.substring(1,2) + F(":00");
  }
  else if (TIME_ZONE == 0)
  {
      tzString = F("Z");
  }
  else if (0 < TIME_ZONE && TIME_ZONE < 10)
  {
      tzString = "+0" + tzString + F(":00");
  }
  else if (10 <= TIME_ZONE && TIME_ZONE <= 24)
  {
      tzString = "+" + tzString + F(":00");
  }
  dateTimeStr += tzString;
  return dateTimeStr;
}

// This sets up the function to be called by the timer with no return of its own.
// This structure is required by the timer library.
// See http://support.sodaq.com/sodaq-one/adding-a-timer-to-schedule-readings/
void showTime(uint32_t ts)
{
  // Retrieve the current date/time
  String dateTime = getDateTime_ISO8601();
  return;
}

// Set-up the RTC Timer events
void setupTimer()
{
  // This tells the timer how often (READ_DELAY) it will repeat some function (showTime)
  timer.every(READ_DELAY, showTime);

  // Instruct the RTCTimer how to get the current time reading (ie, what function to use)
  timer.setNowCallback(getNow);
}

void wakeISR()
{
  //Leave this blank
}

// Sets up the sleep mode (used on device wake-up)
void setupSleep()
{
  pinMode(RTC_PIN, INPUT_PULLUP);
  PcInt::attachInterrupt(RTC_PIN, wakeISR);

  //Setup the RTC in interrupt mode
  rtc.enableInterrupts(RTC_INT_PERIOD);

  //Set the sleep mode
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
}

void sensorsSleep()
{
  // Add any code which your sensors require before sleep
}

void sensorsWake()
{
  // Add any code which your sensors require after waking
}

// Puts the system to sleep to conserve battery life.
void systemSleep()
{
  // This method handles any sensor specific sleep setup
  sensorsSleep();

  // Wait until the serial ports have finished transmitting
  Serial.flush();
  Serial1.flush();

  // The next timed interrupt will not be sent until this is cleared
  rtc.clearINTStatus();

  // Disable ADC
  ADCSRA &= ~_BV(ADEN);

  // Sleep time
  noInterrupts();
  sleep_enable();
  interrupts();
  sleep_cpu();
  sleep_disable();

  // Enable ADC
  ADCSRA |= _BV(ADEN);

  // This method handles any sensor specific wake setup
  sensorsWake();
}

// Initializes the SDcard and prints a header to it
void setupLogFile()
{
  // Initialise the SD card
  if (!SD.begin(SD_SS_PIN))
  {
    Serial.println(F("Error: SD card failed to initialise or is missing."));
  }

  fileName += "_" + getDateTime_ISO8601().substring(0,10) + ".csv";

  // Open the file in write mode
  File logFile = SD.open(fileName, FILE_WRITE);

  logFile.println(LOGGERNAME);
  logFile.println(DATA_HEADER);

  //Close the file to save it
  logFile.close();
}

// Flashess to Mayfly's LED's
void greenred4flash()
{
  for (int i = 1; i <= 4; i++) {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
    delay(50);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    delay(50);
  }
  digitalWrite(RED_LED, LOW);
}

// This function updates the values for any connected sensors. Need to add code for
// Any sensors connected - this example only uses temperature and battery voltage.
bool updateAllSensors()
{
    // Get the temperature from the Mayfly's real time clock
    rtc.convertTemperature();  //convert current temperature into registers
    float tempVal = rtc.getTemperature();
    ONBOARD_TEMPERATURE = tempVal;

    // Get the battery voltage
    float rawBattery = analogRead(BATTERY_PIN);
    ONBOARD_BATTERY = (3.3 / 1023.) * 1.47 * rawBattery;

    // Return true when finished
    return true;
}

// This function generates the JSON data string that becomes the body of the POST request
// TODO:  Figure out how to not use a string here
String generateSensorDataJSON(void)
{
    String jsonString = "{";
    jsonString += "\"sampling_feature\": \"" + String(SAMPLING_FEATURE) + "\", ";
    jsonString += "\"timestamp\": \"" + getDateTime_ISO8601() + "\", ";
    jsonString += "\"" + String(ONBOARD_TEMPERATURE_UUID) + "\": " + String(ONBOARD_TEMPERATURE) + ", ";
    jsonString += "\"" + String(ONBOARD_BATTERY_UUID) + "\": " + String(ONBOARD_BATTERY);
    jsonString += "}";
    return jsonString;
}

// Writes a string to a text file on the SD Card
void logData(String rec)
{
  // Re-open the file
  File logFile = SD.open(fileName, FILE_WRITE);

  // Write the CSV data
  logFile.println(rec);

  // Close the file to save it
  logFile.close();
}

// This function generates the full POST request that gets sent to data.envirodiy.org
// This is only needed for transparent Bee's (ie, WiFi)
void streamPostRequest(Stream & stream)
{
    stream.print(String(F("POST /api/data-stream/  HTTP/1.1")));
    stream.print(String(F("\r\nHost: data.envirodiy.org")));
    stream.print(String(F("\r\nTOKEN: ")) + String(REGISTRATION_TOKEN));
    stream.print(String(F("\r\nContent-Length: ")) + String(generateSensorDataJSON().length()));
    stream.print(String(F("\r\nContent-Type: application/json\r\n\r\n")));
    stream.print(String(generateSensorDataJSON()));
}

// This function makes an HTTP connection to the server and POSTs data - for WIFI
int postDataWiFi(bool redirected = false)
{
    // Serial.println(F("Checking for remaining data in the buffer"));
    printRemainingChars(5, 5000);
    // Serial.println(F("\n"));

    HTTP_RESPONSE result = HTTP_OTHER;

    String resp;

    // Get into command mode
    while (resp != "OK\r")
    {
        delay(1000);
        Serial1.print("+++");
        Serial1.flush();
        resp = Serial1.readString();
        Serial.println("+++");
        Serial.println(resp);
    }

    // Set security to WPA2
    Serial1.println("ATEE 2");
    Serial1.flush();
    resp = Serial1.readString();
    Serial.println("ATEE 2");
    Serial.println(resp);

    // Set the Wifi access point
    Serial1.println("ATID Stroud-Mobile");
    Serial1.flush();
    resp = Serial1.readString();
    Serial.println("ATID Stroud-Mobile");
    Serial.println(resp);

    // Set the Wifi password
    Serial1.println("ATPK phone970");
    Serial1.flush();
    resp = Serial1.readString();
    Serial.println("ATPK phone970");
    Serial.println(resp);

    // Write changes to flash
    Serial1.println("ATWR");
    Serial1.flush();
    resp = Serial1.readString();
    Serial.println("ATWR");
    Serial.println(resp);

    // Apply changes
    Serial1.println("ATAC");
    Serial1.flush();
    resp = Serial1.readString();
    Serial.println("ATAC");
    Serial.println(resp);

    // Make sure we have internet connection
    while (resp != "0")
    {
        Serial1.println("ATAI");
        Serial1.flush();
        resp = Serial1.readStringUntil('\r');
        Serial.println("ATAI");
        Serial.println(resp);
        delay(500);
    }

    // Look up the IP address, just in case it's changed
    Serial1.println("ATLA data.enviroDIY.org");
    Serial1.flush();
    String IP = Serial1.readString();
    Serial.println("ATLA data.enviroDIY.org");
    Serial.println(IP);

    // Set the IP address to whatever the lookup returned
    Serial1.print("ATDL ");
    Serial1.println(IP);
    Serial1.flush();
    resp = Serial1.readString();
    Serial.print("ATDL ");
    Serial.println(IP);
    Serial.println(resp);

    // Set port to 80 (0x50)
    Serial1.println("ATDE 50");
    Serial1.flush();
    resp = Serial1.readString();
    Serial.println("ATDE 50");
    Serial.println(resp);

    // Set mode to transparent
    Serial1.println("ATAP 0");
    Serial1.flush();
    resp = Serial1.readString();
    Serial.println("ATAP 0");
    Serial.println(resp);

    // Write changes to flash
    Serial1.println("ATWR");
    Serial1.flush();
    resp = Serial1.readString();
    Serial.println("ATWR");
    Serial.println(resp);

    // Apply changes
    Serial1.println("ATAC");
    Serial1.flush();
    resp = Serial1.readString();
    Serial.println("ATAC");
    Serial.println(resp);

    // Exit command mode
    Serial1.println("ATCN");
    Serial1.flush();
    resp = Serial1.readString();
    Serial.println("ATCN");
    Serial.println(resp);

    // Send the request to the WiFiBee (it's transparent, just goes as a stream)
    Serial1.println("Hi!");
    Serial1.flush();
    printRemainingChars(5, 5000);
    streamPostRequest(Serial1);
    Serial1.flush();
    printRemainingChars(5, 5000);

    // Send the request to the serial for debugging
    Serial.println(F(" -- Request -- "));
    streamPostRequest(Serial);

    // Add a brief delay for at least the first 12 characters of the HTTP response
    int timeout = COMMAND_TIMEOUT;
    while ((timeout > 0) && Serial1.available() < 12)
    {
      delay(1);
      timeout--;
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
        Serial.println(F("\n -- Response -- "));
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
              else if (memcmp(code, "403", codeBytes) == 0)
              {
                  result = HTTP_FORBIDDEN;
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

// This function makes an HTTP connection to the server and POSTs data - for GPRS
int postDataGPRS(bool redirected = false)
{
    // Serial.println(F("Checking for remaining data in the buffer"));
    printRemainingChars(5, 5000);
    // Serial.println(F("\n"));

    HTTP_RESPONSE result = HTTP_OTHER;

    char url[strlen(HOST_ADDRESS) + strlen(API_ENDPOINT) + 8] = "http://";
    strcat(url,  HOST_ADDRESS);
    strcat(url,  API_ENDPOINT);
    char header[45] = "TOKEN: ";
    strcat(header, REGISTRATION_TOKEN);

    Serial.flush();
    Serial.println(F(" -- Request -- "));
    Serial.println(url);
    Serial.println(header);
    Serial.println(F("Content-Type: application/json"));
    Serial.println(generateSensorDataJSON());
    Serial.flush();

    // Add the needed HTTP Headers
    gprsbee.addHTTPHeaders(header);
    gprsbee.addContentType(F("application/json"));

    // Set up the Response buffer
    char buffer[1024];
    memset(buffer, '\0', sizeof(buffer));

    // Actually make the post request
    bool response = (gprsbee.doHTTPPOSTWithReply(APN, url,
                             generateSensorDataJSON().c_str(),
                             strlen(generateSensorDataJSON().c_str()),
                             buffer, sizeof(buffer)));

    // TODO:  Actually read the response
    if (response)
    {
        result = HTTP_SUCCESS;
    }
    else // Otherwise timeout, no response from server
    {
        result = HTTP_TIMEOUT;
    }

    return result;
}

// Used only for debugging - can be removed
void printPostResult(int result)
{
    switch (result)
    {
        case HTTP_SUCCESS:
        {
            Serial.print(F("\nSucessfully sent data to "));
            Serial.println(HOST_ADDRESS);
        }
        break;

        case HTTP_FAILURE:
        {
            Serial.print(F("\nFailed to send data to "));
            Serial.println(HOST_ADDRESS);
        }
        break;

        case HTTP_FORBIDDEN:
        {
            Serial.print(F("\nAccess to "));
            Serial.print(HOST_ADDRESS);
            Serial.println(F(" forbidden - Check your reguistration token and UUIDs."));
        }
        break;

        case HTTP_TIMEOUT:
        {
            Serial.print(F("\nRequest to "));
            Serial.print(HOST_ADDRESS);
            Serial.println(F(" timed out, no response from server or insufficient signal to send message."));
        }
        break;

        case HTTP_REDIRECT:
        {
            Serial.print(F("\nRequest to "));
            Serial.print(HOST_ADDRESS);
            Serial.println(F(" was redirected."));
        }
        break;

        case HTTP_SERVER_ERROR:
        {
            Serial.print(F("\nRequest to "));
            Serial.print(HOST_ADDRESS);
            Serial.println(F(" aused an internal server error."));
        }
        break;

        default:
        {
            Serial.print(F("\nAn unknown error has occured, and we're pretty confused\n"));
        }
    }
}

// Main setup function
void setup()
{
    // Start the primary serial connection
    Serial.begin(SERIAL_BAUD);
    // Start the serial connection with the *bee
    Serial1.begin(BEE_BAUD);

    // Start the Real Time Clock
    rtc.begin();
    delay(100);

    // Set up pins for the LED's
    pinMode(GREEN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    // Blink the LEDs to show the board is on and starting up
    greenred4flash();

    if (strcasecmp(BEE_TYPE,"GPRS") == 0)
    {
        // Initialize the GPRSBee
        gprsbee.init(Serial1, BEE_CTS_PIN, BEE_DTR_PIN);
        //Comment out the next line when used with GPRSbee Rev.4
        gprsbee.setPowerSwitchedOnOff(true);
        gprsbee.setMinSignalQuality(5);
        // Only to see for debugging - comment this out
        // gprsbee.setDiag(Serial);
    }

    // Set up the log file
    setupLogFile();

    // Setup timer events
    setupTimer();

    // Setup sleep mode
    setupSleep();

    // Print a start-up note to the first serial port
    Serial.println(F("WebSDL Device: EnviroDIY Mayfly"));
    Serial.print(F("Now running "));
    Serial.println(SKETCH_NAME);
    Serial.print(F("Current Mayfly RTC time is: "));
    Serial.println(getDateTime_ISO8601());
}

void loop()
{
    // Update the timer
    timer.update();

    // Check of the current time is an even interval of the test minute
    if (currentminute % testminute == 0)
    {
        // Turn on the LED
        digitalWrite(GREEN_LED, HIGH);
        // Print a few blank lines to show new reading
        Serial.println(F("\n---\n---\n"));
        // Get the sensor value(s), store as string
        updateAllSensors();
        //Save the data record to the log file
        logData(generateSensorDataJSON());
        // Post the data to the WebSDL
        int result;
        if (strcasecmp(BEE_TYPE, "GPRS") == 0)
        {
            result = postDataGPRS();
        };
        if (strcasecmp(BEE_TYPE, "WIFI") == 0)
        {
            result = postDataWiFi();
        };
        // Print the response from the WebSDL
        printPostResult(result);
        // Turn off the LED
        digitalWrite(GREEN_LED, LOW);
        // Advance the timer

        testtimer++;
    }

    // Check if the time is an even unit of the logging interval
    if (testtimer >= LOGGING_INTERVAL)
    {
       testminute = LOGGING_INTERVAL;
    }

    // Sleep
    delay(UPDATE_RATE);
    systemSleep();
}
