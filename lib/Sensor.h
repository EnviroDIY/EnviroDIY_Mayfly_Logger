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

class SensorBase
{
public:
    virtual SENSOR_STATUS setup(void) = 0;
    virtual bool update(void) = 0;

    virtual String getName(void) = 0;
    virtual String getValueAsString(void) = 0;
    virtual SENSOR_STATUS getStatus(void) = 0;
};

template <typename T>
class Sensor : public SensorBase
{
public:
    Sensor(void);
    Sensor(String name);
    virtual ~Sensor();

    virtual SENSOR_STATUS setup(void);
    virtual bool update(void) = 0;

    virtual String getName(void);
    virtual String getValueAsString(void);
    virtual T getValue(void);
    virtual SENSOR_STATUS getStatus(void);
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
String Sensor<T>::getName(void)
{
    return m_name;
}

template <typename T>
SENSOR_STATUS Sensor<T>::getStatus(void)
{
    return m_status;
}

template <typename T>
String Sensor<T>::getValueAsString(void)
{
    return String(m_value);
}

template <typename T>
T Sensor<T>::getValue(void)
{
    return m_value;
}

#endif

