// Sensor.h

#ifndef _SENSOR_h
#define _SENSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

enum SENSOR_STATUS
{
    SENSOR_ERROR,
    SENSOR_READY,
    SENSOR_WAITING,
    SENSOR_UNKNOWN
};


// Defines the "Sensor" Class
class SensorBase
{
public:
    virtual SENSOR_STATUS setup(void) = 0;
    virtual SENSOR_STATUS getStatus(void) = 0;
    virtual bool update(void) = 0;
    virtual bool sleep(void) = 0;
    virtual bool wake(void) = 0;

    virtual int getNumVars(void) = 0;
    virtual String* getVarNames(void) = 0;
    virtual String* getValueAsString(void) = 0;
};


// A template for all sensors.  If any of these functions are not defined
// explicitely within a specific sensor class, the template function will be used.
// The only exception is the "update" function, which MUST be defined.
template <typename T>
class Sensor : public SensorBase
{
public:
    Sensor(void);
    Sensor(String name);
    virtual ~Sensor();

    virtual SENSOR_STATUS setup(void);
    virtual SENSOR_STATUS getStatus(void);
    virtual bool update(void) = 0;
    virtual bool sleep(void);
    virtual bool wake(void);

    virtual int getNumVars(void);
    virtual String* getVarNames(void);
    virtual T* getValues(void);
    virtual String* getValueAsString(void);
protected:
    String m_name;
    T m_value;
    SENSOR_STATUS m_status;
};

template <typename T>
Sensor<T>::Sensor(void)
    : SensorBase(),
    m_name("UNNAMED SENSOR")
{
    m_status = setup();
}

template <typename T>
Sensor<T>::Sensor(String name)
    : m_name(name)
{
    m_status = setup();
}

template <typename T>
Sensor<T>::~Sensor(void)
{
    // Do nothing
}

template <typename T>
SENSOR_STATUS Sensor<T>::setup(void)
{
    return SENSOR_READY;
}

template <typename T>
SENSOR_STATUS Sensor<T>::getStatus(void)
{
    return m_status;
}

template <typename T>
bool Sensor<T>::update(void)
{
    return true;
}

template <typename T>
bool Sensor<T>::sleep(void)
{
    return true;
}

template <typename T>
bool Sensor<T>::wake(void)
{
    return true;
}

template <typename T>
int Sensor<T>::getNumVars(void)
{
    return 1;
}

template <typename T>
String* Sensor<T>::getVarNames(void)
{
    return m_name;
}

template <typename T>
T* Sensor<T>::getValues(void)
{
    return m_value;
}

template <typename T>
String* Sensor<T>::getValueAsString(void)
{
    return String(m_value);
}

#endif
