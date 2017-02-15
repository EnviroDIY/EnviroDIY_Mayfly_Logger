/*
 *This file is part of the DecagonCTD library for Arduino
 *It is dependent on the EnviroDIY SDI-12 library.
 *
 *Work in progress by Sara Damiano taken from code written
 *by Shannon Hicks and templates from USU.
 *
*/

#ifndef _DECAGONCTD_h
#define _DECAGONCTD_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif


class DecagonCTD : public Sensor<float/*SENSORS_DATA_TYPE*/>
{
public:
    DecagonCTD(char CTDaddress, int numReadings);
    bool update();
    String getValueAsString();
    bool wake();
    bool sleep();
private:
    char _CTDaddress
    int _numReadings
};

#endif

