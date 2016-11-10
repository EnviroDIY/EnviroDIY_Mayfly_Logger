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
const String REGISTRATION_TOKEN = "e3f3d179-08bd-4fbb-9710-c7676bf7c732";
const String SAMPLING_FEATURE = "6ea4c89f-bfe0-49c1-b169-8ab85a6507b8";


// -----------------------------------------------
// 3. WebSDL Endpoints for POST requests
// -----------------------------------------------
const String HOST_ADDRESS = "django.uwrl.usu.edu";
const String API_ENDPOINT = "/websdl/api/post_stuff/";


// -----------------------------------------------
// 4. The array that contains all valid sensors
// -----------------------------------------------
SensorBase* SENSOR_LIST[] = {
    new MayFlyOnboardTempSensor(),
    new MayFlyExampleSensor1()
    // new YOUR_SENSOR_NAME_HERE()
};


// -----------------------------------------------
// 5. Misc. Options
// -----------------------------------------------
#define UPDATE_RATE 3000 // milliseconds
#define MAIN_LOOP_DELAY 5000 // milliseconds
#define COMMAND_TIMEOUT 10000 // ms (5000 ms = 5 s)


// -----------------------------------------------
// 6. Board setup info
// -----------------------------------------------
#define XB_BAUD 9600 // XBee BAUD rate (9600 is default)
#define SERIAL_BAUD 57600 // Serial port BAUD rate
