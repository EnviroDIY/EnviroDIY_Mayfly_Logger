// 
// 
// 

#include "MayFlyOnboardTempSensor.h"

MayFlyOnboardTempSensor::MayFlyOnboardTempSensor(void)
    : Sensor<float>("WindDir_Avg")
{
    // Do nothing
}

bool MayFlyOnboardTempSensor::update(void)
{
    // Get the temperature from the Mayfly's real time clock and convert to Farenheit
    rtc.convertTemperature();  //convert current temperature into registers
    float tempVal = rtc.getTemperature();
    m_value = (tempVal * 9.0 / 5.0) + 32.0; // Convert to farenheit
    return true;
}

String MayFlyOnboardTempSensor::getValueAsString()
{
    return String(int(m_value));
}