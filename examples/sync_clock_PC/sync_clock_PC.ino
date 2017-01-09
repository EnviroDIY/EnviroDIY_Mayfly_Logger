#include <Wire.h>  //http://arduino.cc/en/Reference/Wire (included with Arduino IDE)
#include <Sodaq_DS3231.h> //Sodaq's library for the DS3231: https://github.com/SodaqMoja/Sodaq_DS3231

#define SYNC_DELAY 0    // Time between sending the type and clock update
#define TIME_ZONE -5.0  // Time zone difference from UTC
#define TIME_ZONE_SEC (TIME_ZONE * 3600)

void setup()
{
  //Start Serial for serial monitor
  Serial.begin(9600);
  while (!Serial) ; // wait until Arduino Serial Monitor opens
}


void loop()
{
  //Print out current date/time
  Serial.println("Current Date/Time: " + String(getDateTime()) + " (" + String(rtc.now().getEpoch()) + ")");
  Serial.println(Serial.available());

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
  delay(5000);
}


void syncRTCwithBatch()
{
  // Read the timestamp from the PC's batch program
  uint32_t newTs = processSyncMessage();

  if (newTs > 0)
  {
    Serial.println(newTs);

    // Add the timezone difference plus a few seconds
    // to compensate for transmission and processing delay
    newTs += SYNC_DELAY + TIME_ZONE_SEC;

    //Get the old time stamp
    uint32_t oldTs = rtc.now().getEpoch();
    int32_t diffTs = abs(newTs - oldTs);
    int32_t diffTs_abs = abs(diffTs);
    Serial.println("RTC is Off by " + String(diffTs_abs) + " seconds");

    //If time is more than 30s off, update
    if (diffTs_abs > 30)
    {
      //Display old and new time stamps
      Serial.print("Updating RTC, old=" + String(oldTs));
      Serial.println(" new=" + String(newTs));

      //Update the rtc
      rtc.setEpoch(newTs);
    }
    else
    {
      Serial.println("Clock not updated");
    }
  }
}


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

