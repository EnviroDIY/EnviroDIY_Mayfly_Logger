#pragma once

// -----------------------------------------------
// 1. Include all sensors and necessary files here
// -----------------------------------------------
#include "MayFlyOnboardTempSensor.h"
#include "MayFlyExampleSensor1.h"
#include "Sensor.h"


// -----------------------------------------------
// 2. Device registration and sampling features
// -----------------------------------------------
// Skecth file name
const char *SKETCH_NAME = "logging_to_EnviroDIY.ino";

// Data header, for data log file on SD card
const char *LOGGERNAME = "Mayfly 160073";
const char *FILE_NAME = "Mayfly 160073";
const char *DATA_HEADER = "JSON Formatted Data";

// Register your site and get these tokens from data.envirodiy.org
const char *REGISTRATION_TOKEN = "5a3e8d07-8821-4240-91c9-26c610966b2c";
const char *SAMPLING_FEATURE = "39bf098f-d11d-4ea6-9be3-6a073969b019";
const int TIME_ZONE = -5;


// -----------------------------------------------
// 3. Device Connection Options
// -----------------------------------------------
const char* APN = "apn.konekt.io";  // The APN for the GPRSBee
const char *BEE_TYPE = "WIFI";  // The type of XBee, either "GPRS" or "WIFI"


// -----------------------------------------------
// 4. The array that contains all valid sensors
// -----------------------------------------------
SensorBase* SENSOR_LIST[] = {
    new MayFlyOnboardTempSensor(),
    new MayFlyExampleSensor1()
    // new YOUR_SENSOR_NAME_HERE()
};


// -----------------------------------------------
// 5. Timing Options For Logging
// -----------------------------------------------
int LOGGING_INTERVAL = 1;  // How frequently (in minutes) to log data
int READ_DELAY = 1;  // How often (in minutes) the timer wakes up
int UPDATE_RATE = 200; // How frequently (in milliseconds) the logger checks if it should log
int COMMAND_TIMEOUT = 15000;  // How long (in milliseconds) to wait for a server response


// -----------------------------------------------
// 6. WebSDL Endpoints for POST requests
// -----------------------------------------------
const char *HOST_ADDRESS = "data.envirodiy.org";
const char *API_ENDPOINT = "/api/data-stream/";


// -----------------------------------------------
// 7. Board setup info
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
