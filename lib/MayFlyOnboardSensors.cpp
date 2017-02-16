//
//
//

#include "Sensor.h"
#include "MayFlyOnboardSensors.h"


MayFlyOnboardSensors::MayFlyOnboardSensors(void)
    : Sensor<float>("EnviroDIY Mayfly")
{
    // Do nothing
}

bool MayFlyOnboardSensors::update(void)
{
    float m_value[2];

    // Get the temperature from the Mayfly's real time clock
    rtc.convertTemperature();  //convert current temperature into registers
    float tempVal = rtc.getTemperature();
    m_value[0] = tempVal;

    // Get the battery voltage
    int BATTERY_PIN = A6;
    float rawBattery = analogRead(BATTERY_PIN);
    m_value[1] = (3.3 / 1023.) * 1.47 * rawBattery;

    // Return true when finished
    return true;
}

int MayFlyOnboardSensors::getNumVars(void)
{ return 2; }

String* MayFlyOnboardSensors::getVarNames(void)
{
    String m_name[] = {"internal_temp_c", "batt_V"};
    return m_name;
}
