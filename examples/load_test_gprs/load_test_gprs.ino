/**************************************************************************
simple_logging_example.ino
Initial Creation Date: 6/3/2016
Written By:  Jeff Horsburgh (jeff.horsburgh@usu.edu)
Updated By:  Kenny Fryar-Ludwig (kenny.fryarludwig@usu.edu)
Additional Work By:  Sara Damiano (sdamiano@stroudcenter.org)
Development Environment: PlatformIO 3.2.1
Hardware Platform: Stroud Water Resources Mayfly Arduino Datalogger
Radio Module: Sodaq GPRSbee.

This sketch is an example of posting data to the Web Streaming Data Loader
Assumptions:
1. The Mayfly has been registered at http://data.envirodiy.org and the sensor
has been configured. In this example, only temperature and batter voltage are used.

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
#include <avr/wdt.h>
#include <SPI.h>
#include <SD.h>
#include <RTCTimer.h>
#include <Sodaq_DS3231.h>
#include <Sodaq_PcInt.h>
#include <GPRSbee.h>

// -----------------------------------------------
// 2. Device registration and sampling features
// -----------------------------------------------
// Skecth file name
const char *SKETCH_NAME = "load_test_gprs.ino";

// Data header, for data log file on SD card
const char *LOGGERNAME = "Mayfly 160073";
const char *FILE_NAME = "Mayfly 160073";
const char *DATA_HEADER = "JSON Formatted Data";

// Register your site and get these tokens from data.envirodiy.org
const char *REGISTRATION_TOKEN = "a6619e25-53ae-4843-aa96-704619828660";
const char *SAMPLING_FEATURE = "63afe80f-a041-44d6-9df7-52775e973802";
const char *ONBOARD_TEMPERATURE_UUID = "ed75384f-3c5f-42c6-b260-5ce9b12f180c";
const char *ONBOARD_BATTERY_UUID = "9a220558-b3a7-4ff2-8ff4-cd6582cb53c9";
int TIME_ZONE = -5;
const char *UUIDs[] =
{
  "0fc01830-fbcf-437f-b6bc-c17c7510aa95", "f8616ae2-686c-45b4-bd03-bb1a460a0bd2",
  "4d644311-3498-4117-86a0-5a7b34af72e4", "c21ccedb-cfc7-4549-9365-85b2e41f6e5a",
  "ed25ea36-e494-450c-8ae3-f948ea61ef4f", "ad3fba9b-33e7-413c-9499-413adbf12684",
  "85299af3-9c10-4c71-a991-7ce000ff5a6a", "c03f0253-a6bb-4516-a04d-e39842e76695",
  "51588508-8d0a-4df2-a8a8-1b184d541c4b", "78dad08c-265a-4c92-8918-8e48b2cd8783",
  "ac19a9d4-6228-4f9b-a45b-e46acce21e2b", "ab10c168-6f00-4b3b-affd-43a1c46201e5",
  "27b8c5aa-eaac-44d0-b1a4-87c05c818290", "b45a281e-193b-465a-8b58-4b936aa6dba8",
  "6f559986-a3eb-4ac7-9e2a-eeb298a1749d", "d684a77b-a99b-4ba8-b5bc-35ee752ee270",
  "5aa14237-7d94-4f05-8223-a24f88df7460", "abdbc6be-ccf9-4cb6-a288-19f8a9e009a7",
  "b9c9ebca-85b6-4007-8407-59e6f1bf228c", "68bfdd17-646c-468b-8f29-603ddffbab16",
  "dac59b60-efbc-11e6-a380-0050569b4416", "dac6fac9-efbc-11e6-a380-0050569b4416",
  "dac7187a-efbc-11e6-a380-0050569b4416", "dac730d1-efbc-11e6-a380-0050569b4416",
  "dac74bb1-efbc-11e6-a380-0050569b4416", "dac76f76-efbc-11e6-a380-0050569b4416",
  "dac82103-efbc-11e6-a380-0050569b4416", "dac838e0-efbc-11e6-a380-0050569b4416",
  "dac850f7-efbc-11e6-a380-0050569b4416", "dac86952-efbc-11e6-a380-0050569b4416",
  "dac88223-efbc-11e6-a380-0050569b4416", "dac8a527-efbc-11e6-a380-0050569b4416",
  "dac8bd7d-efbc-11e6-a380-0050569b4416", "dac8d53a-efbc-11e6-a380-0050569b4416",
  "dac8eceb-efbc-11e6-a380-0050569b4416", "dac90507-efbc-11e6-a380-0050569b4416",
  "dac91ce7-efbc-11e6-a380-0050569b4416", "dac934a0-efbc-11e6-a380-0050569b4416",
  "dac94c26-efbc-11e6-a380-0050569b4416", "dac963a0-efbc-11e6-a380-0050569b4416",
  "dac97be7-efbc-11e6-a380-0050569b4416", "dac99390-efbc-11e6-a380-0050569b4416",
  "dac9aadf-efbc-11e6-a380-0050569b4416", "dac9c288-efbc-11e6-a380-0050569b4416",
  "dac9d9bb-efbc-11e6-a380-0050569b4416", "dac9f07c-efbc-11e6-a380-0050569b4416",
  "daca080e-efbc-11e6-a380-0050569b4416", "daca2055-efbc-11e6-a380-0050569b4416",
  "daca3641-efbc-11e6-a380-0050569b4416", "daca4b35-efbc-11e6-a380-0050569b4416",
  "dac58b60-efbc-11e6-a380-0050569b4416"
};

// -----------------------------------------------
// 3. WebSDL Endpoints for POST requests
// -----------------------------------------------
const char *HOST_ADDRESS = "data.envirodiy.org";
const char *API_ENDPOINT = "/api/data-stream/";

// -----------------------------------------------
// 4. Misc. Options
// -----------------------------------------------
int LOGGING_INTERVAL = 1;  // How frequently (in minutes) to log data
int READ_DELAY = 1;  // How often (in minutes) the timer wakes up
int UPDATE_RATE = 200; // How frequently (in milliseconds) the logger checks if it should log
int COMMAND_TIMEOUT = 15000;  // How long (in milliseconds) to wait for a server response

// -----------------------------------------------
// 5. Board setup info
// -----------------------------------------------
int SERIAL_BAUD = 9600;  // Serial port BAUD rate
int BEE_BAUD = 9600;  // Bee BAUD rate (9600 is default)
const char *BEE_TYPE = "GPRS";  // The type of XBee, either "GPRS" or "WIFI"
int BEE_DTR_PIN = 23;  // Bee DTR Pin (Data Terminal Ready - used for sleep)
int BEE_CTS_PIN = 19;   // Bee CTS Pin (Clear to Send)
int GREEN_LED = 8;  // Pin for the green LED
int RED_LED = 9;  // Pin for the red LED
const char* APN = "apn.konekt.io";  // The APN for the GPRSBee

int RTC_PIN = A7;  // RTC Interrupt pin
#define RTC_INT_PERIOD EveryMinute  //The interrupt period on the RTC

int BATTERY_PIN = A6;    // select the input pin for the potentiometer

int SD_SS_PIN = 12;  // SD Card Pin

// -----------------------------------------------
// 6. Setup variables
// -----------------------------------------------
float ONBOARD_TEMPERATURE = 0;  // Variable to store the temperature result in
float ONBOARD_BATTERY;  // variable to store the value coming from the sensor

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
            // char netChar = Serial1.read();
            // Serial.print(netChar);
            Serial1.read();
            delay(timeDelay);
        }
        delay(timeDelay);
    }
    Serial1.flush();
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
  dateTimeStr.replace(" ", "T");
  String tzString = String(TIME_ZONE);
  if (-24 <= TIME_ZONE && TIME_ZONE <= -10)
  {
      tzString += ":00";
  }
  else if (-10 < TIME_ZONE && TIME_ZONE < 0)
  {
      tzString = tzString.substring(0,1) + "0" + tzString.substring(1,2) + ":00";
  }
  else if (TIME_ZONE == 0)
  {
      tzString = "Z";
  }
  else if (0 < TIME_ZONE && TIME_ZONE < 10)
  {
      tzString = "+0" + tzString + ":00";
  }
  else if (10 <= TIME_ZONE && TIME_ZONE <= 24)
  {
      tzString = "+" + tzString + ":00";
  }
  dateTimeStr += tzString;
  return dateTimeStr;
}

// This sets up the function to be called by the timer with no raturn of its own.
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
// Any sensors connected - this example only uses temperature.
bool updateAllSensors()
{
    // Get the temperature from the Mayfly's real time clock and convert to Farenheit
    rtc.convertTemperature();  //convert current temperature into registers
    float tempVal = rtc.getTemperature();
    ONBOARD_TEMPERATURE = tempVal;

    float rawBattery = analogRead(BATTERY_PIN);
    ONBOARD_BATTERY = (3.3 / 1023.) * 1.47 * rawBattery;
    return true;
}

// This function generates the JSON data string that becomes the body of the POST request
String generateSensorDataString(void)
{
    char jsonString[] = "{\"sampling_feature\": \"";
    strcat(jsonString, SAMPLING_FEATURE);
    strcat(jsonString, ", \" \"timestamp\": \"");
    strcat(jsonString, getDateTime_ISO8601().c_str());
    strcat(jsonString, "\", \"");
    strcat(jsonString, ONBOARD_TEMPERATURE_UUID);
    strcat(jsonString, "\", ");
    char valToChar[7];
    strcat(jsonString, dtostrf(ONBOARD_TEMPERATURE,6,2,valToChar));
    int sensor_num = 0;
    while(sensor_num < 51)
    {
      char valToChar2[13];
      dtostrf(random(1000000000)/100000000.0,12,10,valToChar2);
      strcat(jsonString, "\", \"");
      strcat(jsonString, UUIDs[sensor_num]);
      strcat(jsonString, "\", ");
      strcat(jsonString, valToChar);
      sensor_num++;
    }
    strcat(jsonString, "\", \"");
    strcat(jsonString, ONBOARD_BATTERY_UUID);
    strcat(jsonString, "\", ");
    strcat(jsonString, dtostrf(ONBOARD_BATTERY,6,2,valToChar));
    strcat(jsonString, "}");
    return jsonString;
}

// Initializes the SDcard and prints a header to it
void setupLogFile()
{
  // Initialise the SD card
  if (!SD.begin(SD_SS_PIN))
  {
    Serial.println("Error: SD card failed to initialise or is missing.");
    //Hang
    //  while (true);
  }

  // Check if the file already exists
  bool oldFile = SD.exists(FILE_NAME);

  // Open the file in write mode
  File logFile = SD.open(FILE_NAME, FILE_WRITE);

  // Add header information if the file did not already exist
  if (!oldFile)
  {
    logFile.println(LOGGERNAME);
    logFile.println(DATA_HEADER);
  }

  //Close the file to save it
  logFile.close();
}

// Writes a string to a text file on the SDCar
void logData(String rec)
{
  // Re-open the file
  File logFile = SD.open(FILE_NAME, FILE_WRITE);

  // Write the CSV data
  logFile.println(rec);

  // Close the file to save it
  logFile.close();
}

// This generates the POST headers.
char generatePostHeaders(char *dataString)
{
    char header[] = "TOKEN: ";
    strcat(header, REGISTRATION_TOKEN);
    if (strcasecmp(BEE_TYPE, "WIFI")== 0)  // Add additional headers for the WiFi
    {
        strcat(header, "\r\nCache-Control: no-cache\r\n");
        strcat(header, "Content-Length: ");
        char lenDataString[4];
        strcat(header, itoa(strlen(dataString), lenDataString, 10));
        strcat(header, "\r\n");
        strcat(header, "Content-Type: application/json\r\n");
    }
    return header;
}

// This function generates the full POST request that gets sent to data.envirodiy.org
char generatePostRequest(void)
{
    char request[] = "POST ";
    strcat(request, API_ENDPOINT);
    strcat(request, " HTTP/1.1\r\n");
    strcat(request, "Host: ");
    strcat(request, HOST_ADDRESS);
    strcat(request, "\r\n");
    strcat(request, generatePostHeaders(generateSensorDataString()));
    strcat(request, "\r\n");
    strcat(request, generateSensorDataString());
    strcat(request, "\r\n\r\n");
    return request;
}

// This function makes an HTTP connection to the server and POSTs data - for WIFI
int postDataWiFi(bool redirected = false)
{
    // Serial.println("Checking for remaining data in the buffer");
    printRemainingChars(5, 5000);
    // Serial.println("\n");

    HTTP_RESPONSE result = HTTP_OTHER;

    Serial1.flush();
    Serial1.print(generatePostRequest().c_str());
    Serial1.flush();


    Serial.flush();
    Serial.println(" -- Request -- ");
    Serial.print(generatePostRequest().c_str());
    Serial.flush();

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
    // Serial.println("Checking for remaining data in the buffer");
    printRemainingChars(5, 5000);
    // Serial.println("\n");

    HTTP_RESPONSE result = HTTP_OTHER;

    char url[] = "http://";
    strcat(url,  HOST_ADDRESS);
    strcat(url,  API_ENDPOINT);
    String headers = generatePostHeaders(generateSensorDataString());

    Serial.flush();
    Serial.println(" -- Request -- ");
    Serial.println(url);
    Serial.println(headers);
    Serial.println("Content-Type: application/json");
    Serial.println(generateSensorDataString());
    Serial.flush();

    // Add the needed HTTP Headers
    gprsbee.addHTTPHeaders(headers);
    gprsbee.addContentType("application/json");

    // Set up the Response buffer
    char buffer[1024];
    memset(buffer, '\0', sizeof(buffer));

    // Actually make the post request
    bool response = (gprsbee.doHTTPPOSTWithReply(APN, url.c_str(),
                             generateSensorDataString().c_str(),
                             strlen(generateSensorDataString().c_str()),
                             buffer, sizeof(buffer)));

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
            Serial.println("\nSucessfully sent data to " + HOST_ADDRESS + "\n");
        }
        break;

        case HTTP_FAILURE:
        {
            Serial.println("\nFailed to send data to " + HOST_ADDRESS + "\n");
        }
        break;

        case HTTP_FORBIDDEN:
        {
            Serial.println("\nAccess to " + HOST_ADDRESS + " forbidden - Check your reguistration token and UUIDs.\n");
        }
        break;

        case HTTP_TIMEOUT:
        {
            Serial.println("\nRequest to " + HOST_ADDRESS + " timed out, no response from server or insufficient signal to send message.\n");
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
        gprsbee.setMinSignalQuality(7);
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
    Serial.println("WebSDL Device: EnviroDIY Mayfly");
    Serial.println("Now running " + SKETCH_NAME);
    Serial.println("Current Mayfly RTC time is: " + getDateTime_ISO8601());
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
        Serial.println("\n---\n---\n");
        // Get the sensor value(s), store as string
        updateAllSensors();
        //Save the data record to the log file
        logData(generateSensorDataString());
        // Post the data to the WebSDL
        int result;
        if (BEE_TYPE == "GPRS")
        {
            result = postDataGPRS();
        };
        if (BEE_TYPE == "WIFI")
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
