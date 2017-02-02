/* ***********************************************************************************************
 * 
 * Sara Damiano - January 12, 2017
 * 
 * This script is intended to synchronize the RTC clock chip of an EnviroDIY Mayfly.
 * After uploading this script to your Mayfly, the time can be set by either manually sending 
 * a "T" followed by a unix time stamp to the Mayfly (ie, T1451606400) or by running the
 * sync_clock_PC.py python script which will automatically synchronize the RTC to UTC based
 * on the computer's clock or NTP (if internet connection is available).
 * 
 * This script is meant to be use on a naked EnviroDIY Mayfly board with no connection other than
 * directly to the computer via microUSB.  If a GPRSbee or other internet/radio access shields are 
 * attached to they Mayfly, it is more efficient to utilize the internet directly to synchronize the
 * RTC chip rather than using this script.  An example of that type of script is available on Sodaq's
 * website.
 * 
 * This script requires the wire library generally built into the Arduino IDE and the Sodaq DS3231
 * library, linked below.
 *
 *********************************************************************************************** */

#include <Wire.h>  //http://arduino.cc/en/Reference/Wire (included with Arduino IDE)
#include <Sodaq_DS3231.h> //Sodaq's library for the DS3231: https://github.com/SodaqMoja/Sodaq_DS3231

String getDateTime()
{
  String dateTimeStr;

  //Create a DateTime object from the current time
  DateTime dt(rtc.makeDateTime(rtc.now().getEpoch()));

  //Convert it to a String
  dt.addToString(dateTimeStr);

  return dateTimeStr;
}


/*  code to process time sync messages from the serial port   */
#define TIME_HEADER  "T"   // Header tag for serial time sync message

unsigned long processSyncMessage() {
  unsigned long pctime = 0L;
  const unsigned long DEFAULT_TIME = 1451606400; // Jan 1 2016 00:00:00.000
  const unsigned long MAX_TIME = 2713910400; // Jan 1 2056 00:00:00.000

  if (Serial.find(TIME_HEADER)) {
    pctime = Serial.parseInt();
    Serial.println("Received:" + String(pctime));
    if ( pctime < DEFAULT_TIME) // check the value is a valid time (greater than Jan 1 2016)
    {
      Serial.println("Time out of range");
      pctime = 0L; // return 0 to indicate that the time is not valid
    }
    if ( pctime > MAX_TIME) // check the value is a valid time (greater than Jan 1 2016)
    {
      Serial.println("Time out of range");
      pctime = 0L; // return 0 to indicate that the time is not valid
    }
  }
  return pctime;
}


void syncRTCwithBatch()
{
  // Read the timestamp from the PC's batch program
  uint32_t newTs = processSyncMessage();

  if (newTs > 0)
  {
    //Serial.println(newTs);

    // Add the timezone difference plus a few seconds
    // to compensate for transmission and processing delay
    //newTs += SYNC_DELAY + TIME_ZONE_SEC;

    //Get the old time stamp and print out difference in times
    uint32_t oldTs = rtc.now().getEpoch();
    int32_t diffTs = newTs - oldTs;
    int32_t diffTs_abs = abs(diffTs);
    Serial.println("RTC is Off by " + String(diffTs_abs) + " seconds");

    //Display old and new time stamps
    Serial.print("Updating RTC, old = " + String(oldTs));
    Serial.println(" new = " + String(newTs));

    //Update the rtc
    rtc.setEpoch(newTs);
  }
}


void setup()
{
  //Start Serial for serial monitor
  Serial.begin(57600);
  while (!Serial) ; // wait until Arduino Serial Monitor opens
  Serial.println("Running sketch: sync_clock_PC.ino");
}


void loop()
{
  //Print out current date/time
  Serial.println("Current RTC Date/Time: " + String(rtc.now().dayOfWeek()) + String(getDateTime()) + " (" + String(rtc.now().getEpoch()) + ")");

  if (Serial.available())
  {
    //Sync time
    syncRTCwithBatch();
  }
  // Empty the serial buffer
  while (Serial.available() > 0)
  {
    Serial.read();
  }

  delay(1000);
}

