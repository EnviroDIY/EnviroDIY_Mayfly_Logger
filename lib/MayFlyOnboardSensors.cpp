//
//
//

#include "Sensor.h"
#include "MayFlyOnboardSensors.h"


// MayFlyOnboardSensors::MayFlyOnboardSensors(void)
//     : Sensor<float>("EnviroDIY Mayfly")
// {
//     // Do nothing
// }

bool MayFlyOnboardSensors::update(void)
{
    // Get the temperature from the Mayfly's real time clock
    rtc.convertTemperature();  //convert current temperature into registers
    float tempVal = rtc.getTemperature();
    float m_value_temp = tempVal;

    // Get the battery voltage
    int BATTERY_PIN = A6;
    float rawBattery = analogRead(BATTERY_PIN);
    float m_value_battery = (3.3 / 1023.) * 1.47 * rawBattery;

    // Return true when finished
    return true;
}

String MayFlyOnboardSensors::getSensorName(void)
{
    m_name = "EnviroDIY Mayfly";
    return m_name;
}


String MayFlyOnboardTemp::getVarName(void)
{
    sensor = "temperatureDatalogger";
    return sensor;
}


String MayFlyOnboardTemp::getVarUnit(void)
{
    unit = "degreeCelsius";
    return unit;
}

float MayFlyOnboardTemp::getValue(void)
{
    return m_value_temp;
}


String MayFlyOnboardBattery::getVarName(void)
{
    sensor = "batteryVoltage";
    return sensor;
}


String MayFlyOnboardBattery::getVarUnit(void)
{
    unit = "Volt";
    return unit;
}

float MayFlyOnboardBattery::getValue(void)
{
    return m_value_battery;
}
