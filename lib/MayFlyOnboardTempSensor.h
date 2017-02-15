// MayFlyOnboardTempSensor.h

#ifndef _MAYFLYONBOARDTEMPSENSOR_h
#define _MAYFLYONBOARDTEMPSENSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Sensor.h"
#include "Sodaq_DS3231.h"
#include <Wire.h>

class MayFlyOnboardTempSensor: public Sensor<float>
{
public:
    MayFlyOnboardTempSensor(void);
    bool update(void);
    String getValueAsString();
};

#endif

