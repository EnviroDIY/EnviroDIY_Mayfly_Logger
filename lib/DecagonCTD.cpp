/*
 *This file is part of the DecagonCTD library for Arduino
 *It is dependent on the EnviroDIY SDI-12 library.
 *
 *Work in progress by Sara Damiano taken from code written
 *by Shannon Hicks and templates from USU.
 *
*/

#include "DecagonCTD.h"
#include "SDI12_Mod.h"

char CTDaddress = '1';      //for one sensor on channel '1'
float CTDtempC, CTDdepthmm, CTDcond;

#define DATAPIN 7         // change to the proper pin for sdi-12 data pin, pin 7 on shield 3.0
int SwitchedPower = 22;    // sensor power is pin 22 on Mayfly
SDI12 mySDI12(DATAPIN);


DecagonCTD::DecagonCTD(char CTDaddress)
  : Sensor<float/*SENSORS_DATA_TYPE*/>("specificConductance")
  : Sensor<float/*SENSORS_DATA_TYPE*/>("temperature")
  : Sensor<float/*SENSORS_DATA_TYPE*/>("waterDepth")
{
  _CTDaddress = CTDaddress;
  _numReadings = numReadings
}


// Uses SDI-12 to communicate with a Decagon Devices CTD
bool DecagonCTD::update()
{
  CTDdepthmm = 0;
  CTDtempC = 0;
  CTDcond = 0;

  for (int j = 0; j < _numReadings; j++) {   //averages x readings in this one loop

    String command = "";
    command += _CTDaddress;
    command += "M!"; // SDI-12 measurement command format  [address]['M'][!]
    mySDI12.sendCommand(command);
    delay(500); // wait a while
    mySDI12.flush(); // we don't care about what it sends back

    command = "";
    command += _CTDaddress;
    command += "D0!"; // SDI-12 command to get data [address][D][dataOption][!]
    mySDI12.sendCommand(command);
    delay(500);
    if (mySDI12.available() > 0) {
      float junk = mySDI12.parseFloat();
      int x = mySDI12.parseInt();
      float y = mySDI12.parseFloat();
      int z = mySDI12.parseInt();

      CTDdepthmm += x;
      CTDtempC += y;
      CTDcond += z;
    }

    mySDI12.flush();
  }     // end of averaging loop

  float numRead_f = (float) _numReadings;
  CTDdepthmm /= numRead_f ;
  CTDtempC /= numRead_f ;
  CTDcond /= numRead_f ;
  
  return true;
}

String DecagonCTD::getValueAsString()
{
  return String(int(m_value));
}


bool DecagonCTD::sleep(void)
{
  // Put your sensor to sleep, if applicable
  // Return true if successful
  return true;
}


bool DecagonCTD::wake(void)
{
  // Wake your sensor up, if applicable
  // Return true if successful
  return true;
}

