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

class MayFlyOnboardSensors: public Sensor<float>
{
public:
    MayFlyOnboardSensors(void);
    bool update(void);

    int getNumVars(void);
    String* getVarNames(void);
    float* getValues(void);
private:
    int BATTERY_PIN;
};

#endif
