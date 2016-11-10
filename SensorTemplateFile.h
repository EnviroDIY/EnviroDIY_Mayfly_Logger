// SensorTemplateFile.h

#ifndef _SENSOR_TEMPLATE_FILE_h
#define _SENSOR_TEMPLATE_FILE_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "Sensor.h"
#include "Sodaq_DS3231.h"
#include <Wire.h>

class SensorTemplateFile : public Sensor<float/*SENSORS_DATA_TYPE*/>
{
public:
    SensorTemplateFile(void);
    bool update(void);
    String getValueAsString();
};

#endif

