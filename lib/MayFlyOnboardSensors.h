// MayFlyOnboardSensors.h

#ifndef _MayFlyOnboardSensors_h
#define _MayFlyOnboardSensors_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Sensor.h"
#include <Sodaq_DS3231.h>
#include <Wire.h>

// The main class for the Mayfly
class MayFlyOnboardSensors: public Sensor<float>
{
public:
    MayFlyOnboardSensors(void);
    bool update(void);
    String getSensorName(void);
private:
    int BATTERY_PIN;
protected:
    String sensor;
    String unit;
    float m_value_temp;
    float m_value_battery;
};


class MayFlyOnboardTemp: public MayFlyOnboardSensors
{
public:
    String getVarName(void);
    String getVarUnit(void);
    float getValue(void);
    String getValueAsString(void);
};


class MayFlyOnboardBattery: public MayFlyOnboardSensors
{
public:
    String getVarName(void);
    String getVarUnit(void);
    float getValue(void);
    String getValueAsString(void);
};

#endif
