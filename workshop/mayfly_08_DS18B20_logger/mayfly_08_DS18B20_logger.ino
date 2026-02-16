//Sample sketch for logging DS18B20 temperature data to microSD card on EnviroDIY Mayfly 

#include <Wire.h>
#include "Sodaq_DS3231.h"
#include <SPI.h>
#include <SD.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 5
//#define TEMPERATURE_PRECISION 11 

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

#define SD_SS_PIN 12

#define FILE_NAME "Datafile.txt"
//Data header
#define LOGGERNAME "Mayfly DS18B20 Temperature Logger"  
#define DATA_HEADER "DateTime_UTC,Loggertime,BoardTempC,BatteryV,Sensor_Temp_C"

String dataRec = "";
int currentminute;
long currentepochtime = 0;
float boardtemp = 0.0;

int batteryPin = A6;    // select the input pin for the potentiometer
int batterysenseValue = 0;  // variable to store the value coming from the sensor
float batteryvoltage;       // the calculated battery voltage in volts

void setup () 
{
    Serial.begin(57600);
    Serial.println("Mayfly Workshop Multi-Sensor & microSD Logger");
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    greenred4flash();   //blink the LEDs to show the board is on    
    digitalWrite(8, HIGH);
    Wire.begin();
    rtc.begin();
    setupLogFile();         
    sensors.begin();
//    sensors.setResolution(tempDeviceAddress, TEMPERATURE_PRECISION);
}

void loop () 
{ 
     dataRec = createDataRecord();
     sensormeasurements();
     logData(dataRec);
     Serial.print("Data Record: ");
     Serial.println(dataRec);      
     String dataRec = ""; 
  
     delay(5000);
}

void setupLogFile()
{
  //Initialise the SD card
  if (!SD.begin(SD_SS_PIN))
  {
    Serial.println("Error: SD card failed to initialise or is missing.");

    for (int r=1; r <= 10; r++)
    {
      digitalWrite(9, HIGH);
      delay(400);
      digitalWrite(9, LOW);
      delay(200);
    }
  
  }
  
  //Check if the file already exists
  bool oldFile = SD.exists(FILE_NAME);  
  
  //Open the file in write mode
  File logFile = SD.open(FILE_NAME, FILE_WRITE);
  
  //Add header information if the file did not already exist
  if (!oldFile)
  {
    logFile.println(LOGGERNAME);
    logFile.println(DATA_HEADER);
  }
  
  //Close the file to save it
  logFile.close();  
}


void logData(String rec)
{
  //Re-open the file
  File logFile = SD.open(FILE_NAME, FILE_WRITE);
  
  //Write the CSV data
  logFile.println(rec);
  
  //Close the file to save it
  logFile.close();  
}

String createDataRecord()
{
    //Create a String type data record in csv format
    String data = getDateTime();
    data += ",";
     
    rtc.convertTemperature();          //convert current temperature into registers
    boardtemp = rtc.getTemperature(); //Read temperature sensor value
    
    batterysenseValue = analogRead(batteryPin);
    batteryvoltage = (3.3/1023.) * 4.7 * batterysenseValue;
    
    data += currentepochtime;
    data += ",";

    addFloatToString(data, boardtemp, 3, 1);    //float   
    data += ",";  
    addFloatToString(data, batteryvoltage, 4, 2);
  
//  Serial.print("Data Record: ");
//  Serial.println(data);
  return data;
}


static void addFloatToString(String & str, float val, char width, unsigned char precision)
{
  char buffer[10];
  dtostrf(val, width, precision, buffer);
  str += buffer;
}

void sensormeasurements()
{
      sensors.requestTemperatures(); // Send the command to get temperatures
      float tempC = sensors.getTempCByIndex(0);

      dataRec += ",";
      addFloatToString(dataRec, tempC, 5, 3);
}

String getDateTime()
{
  String dateTimeStr;
  
  //Create a DateTime object from the current time
  DateTime dt(rtc.makeDateTime(rtc.now().getEpoch()));

  currentepochtime = (dt.get());    //Unix time in seconds 

  currentminute = (dt.minute());
  //Convert it to a String
  dt.addToString(dateTimeStr); 
  return dateTimeStr;  
}

uint32_t getNow()
{
  currentepochtime = rtc.now().getEpoch();
  return currentepochtime;
}

void greenred4flash()
{
  for (int i=1; i <= 4; i++){
  digitalWrite(8, HIGH);   
  digitalWrite(9, LOW);
  delay(50);
  digitalWrite(8, LOW);
  digitalWrite(9, HIGH);
  delay(50);
  }
  digitalWrite(9, LOW);
}
