#pragma once

// -----------------------------------------------
// 1. Include all sensors and necessary files here
// -----------------------------------------------
#include "Sensor.h"
#include <MayFlyOnboardSensors.h>


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

const char *UUIDs[] =
{
"fec11d32-0658-4ef0-8a27-bdffa2104e31", "a7329b1b-b002-4fa8-afba-ae83b82ab8e9"
};


// -----------------------------------------------
// 3. Device Connection Options
// -----------------------------------------------
const char *BEE_TYPE = "WIFI";  // The type of XBee, either "GPRS" or "WIFI"
const char* APN = "apn.konekt.io";  // The APN for the GPRSBee


// -----------------------------------------------
// 4. The array that contains all valid sensors
// -----------------------------------------------
SensorBase *SENSOR_LIST[] = {
    new MayFlyOnboardTemp(),
    new MayFlyOnboardBattery()
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
const int SERIAL_BAUD = 9600;  // Serial port BAUD rate
const int BEE_BAUD = 9600;  // Bee BAUD rate (9600 is default)
const int BEE_DTR_PIN = 23;  // Bee DTR Pin (Data Terminal Ready - used for sleep)
const int BEE_CTS_PIN = 19;   // Bee CTS Pin (Clear to Send)
const int GREEN_LED = 8;  // Pin for the green LED
const int RED_LED = 9;  // Pin for the red LED

const int DATAPIN = 7;         // change to the proper pin for sdi-12 data pin, pin 7 on shield 3.0
const int SwitchedPower = 22;    // sensor power is pin 22 on Mayfly

const int RTC_PIN = A7;  // RTC Interrupt pin
#define RTC_INT_PERIOD EveryMinute  //The interrupt period on the RTC

const int SD_SS_PIN = 12;  // SD Card Pin
