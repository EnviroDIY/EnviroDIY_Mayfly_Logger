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
#include <RTCTimer.h>
#include <Sodaq_DS3231.h>
#include <Sodaq_PcInt.h>

// -----------------------------------------------
// 2. Device registration and sampling features
// -----------------------------------------------
const String REGISTRATION_TOKEN = "9e8bbb13-1b71-4720-b930-1d7b0d7602b0";
const String SAMPLING_FEATURE = "a8bff3ec-4ca3-4d13-b1e2-e5effeceb4a1";

// -----------------------------------------------
// 3. WebSDL Endpoints for POST requests
// -----------------------------------------------
const String HOST_ADDRESS = "data.envirodiy.org";
const String API_ENDPOINT = "/api/data-stream/";

// -----------------------------------------------
// 4. Misc. Options
// -----------------------------------------------
#define UPDATE_RATE 30000 // milliseconds
#define MAIN_LOOP_DELAY 5000 // milliseconds
#define COMMAND_TIMEOUT 10000 // ms (5000 ms = 5 s)
#define READ_DELAY 1

// -----------------------------------------------
// 5. Board setup info
// -----------------------------------------------
#define SERIAL_BAUD 57600 // Serial port BAUD rate
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

    //Serial1.flush();
    //Serial1.print(requestString.c_str());
    //Serial1.flush();

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


// This function returns the datetime from the realtime clock as a string
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
String generateSensorDataString(void)
{
    String jsonString = "{ ";
    jsonString += "\"timestamp\": \"" + getDateTime() + "\", ";
    jsonString += "\"sampling_feature\": \"" + SAMPLING_FEATURE + "\", ";
    jsonString += "\"71408b33-b486-436c-8ed6-7382491f1e12\": " + String(int(ONBOARD_TEMPERATURE));
    jsonString += " }";
    return jsonString;
}


// Helper function to retrieve and display the current time
void showTime(uint32_t ts)
{
  // Retrieve and display the current date/time
  String dateTime = getDateTime();
  //Serial.println(dateTime);
}


// Set-up the RTC Timer events
void setupTimer()
{
  // Schedule the wakeup every minute
  timer.every(READ_DELAY, showTime);

  // Instruct the RTCTimer how to get the current time reading
  timer.setNowCallback(rtc.now().getEpoch());
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

  // Enale ADC
  ADCSRA |= _BV(ADEN);

  // This method handles any sensor specific wake setup
  sensorsWake();
}


// Flashess to Mayfly's LED's
void greenred4flash()
{
  for (int i = 1; i <= 4; i++) {
    digitalWrite(8, HIGH);
    digitalWrite(9, LOW);
    delay(50);
    digitalWrite(8, LOW);
    digitalWrite(9, HIGH);
    delay(50);
  }
  digitalWrite(9, LOW);
}

// Main setup function
void setup()
{
  // Start the primary serial connection to the PC
  Serial.begin(SERIAL_BAUD);   // Start the serial connections
  // Start the serial connection with the *bee
  //Serial1.begin(XB_BAUD);      // XBee hardware serial connection

  // Start the Real Time Clock
  rtc.begin();
  delay(100);

  // Set up pins for the LED's (Mayfly pins 8 & 9)
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);

  // Blink the LEDs to show the board is on and starting up
  greenred4flash();

  // Setup timer events
  setupTimer();

  // Setup sleep mode
  setupSleep();

  // Print a start-up note to the first serial port
  Serial.println("WebSDL Device: EnviroDIY Mayfly\n");

}

// Continuously Running Loop
void loop()
{
    // Update the timer
    timer.update();

    // Check of the current time is an even interval of the test minute
    if (currentminute % testminute == 0)
    {
        // Blink the LED
        digitalWrite(8, HIGH);
        // Print a few blank lines to show new reading
        Serial.println("\n---\n---\n");
        updateAllSensors(); // get the sensor value, store as string
        String request = generatePostRequest(generateSensorDataString());
        int result = postData(request);
        printPostResult(result);
        testtimer++;
    }

      // Check if the time is an interval of 5 minutes
      if (testtimer >= 5)
      { testminute = 5;
      }

    delay(MAIN_LOOP_DELAY);

    //Sleep
    systemSleep();
}
