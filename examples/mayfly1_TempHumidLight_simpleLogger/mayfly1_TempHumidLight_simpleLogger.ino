//Sample sketch for logging light and temp/humidity data to microSD card on EnviroDIY Mayfly v1.0
//This sketch does not put the logger to sleep; it is constantly power, so it's not intended for long-term deployments


#include <Wire.h>
#include "Sodaq_DS3231.h"
#include <SPI.h>
#include <SD.h>
 
#include "Adafruit_SHT4x.h"

Adafruit_SHT4x sht4 = Adafruit_SHT4x();

#define SD_SS_PIN 12

#define FILE_NAME "Datafile.txt"
#define LOGGERNAME "Mayfly Light/Humidity/Temperature Logger"  
#define DATA_HEADER "DateTime_UTC,Loggertime,BatteryV,Humidity_percent,Temp_C,Light"

String dataRec = "";
int currentminute;
long currentepochtime = 0;
float boardtemp = 0.0;
float lightpercent = 0;

int batteryPin = A6;        // select the input pin for the potentiometer
int batterysenseValue = 0;  // variable to store the value coming from the sensor
float batteryvoltage;       // the calculated battery voltage in volts

void setup () 
{
    Serial.begin(57600);
    Serial.println("Mayfly SHT40-Light Logger");
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(20, OUTPUT);
    greenred4flash();   //blink the LEDs to show the board is on    
    Wire.begin();
    rtc.begin();
    setupLogFile();
    sht4.begin();
    sht4.setPrecision(SHT4X_HIGH_PRECISION);
    sht4.setHeater(SHT4X_NO_HEATER);  

}

void loop () 
{ 
     digitalWrite(8, HIGH);   // turn on the green LED to let you know a sample is being taken
     dataRec = createDataRecord();
     int light = analogRead(A4);     // read the analog light sensor
     lightpercent = light/10.23;     // scales light reading as a percentage
     sensors_event_t humidity, temp;
     sht4.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
     dataRec += ",";
     addFloatToString(dataRec, humidity.relative_humidity, 4, 2);
     dataRec += ",";
     addFloatToString(dataRec, temp.temperature, 4, 2);
     dataRec += ",";
     addFloatToString(dataRec, lightpercent, 2, 1);
     dataRec += light;
     delay(10);
     logData(dataRec);
     Serial.print("Data Record: ");
     Serial.println(dataRec);       // print the data record to the serial monitor
     String dataRec = ""; 
     digitalWrite(8, LOW);  
     delay(1000);
}

void setupLogFile()
{
  //Initialise the SD card
  if (!SD.begin(SD_SS_PIN))
  {
    Serial.println("Error: SD card failed to initialise or is missing.");

    for (int r=1; r <= 10; r++)   // flash the red LED 10 times to let you know there's no memory card
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
    
    batterysenseValue = analogRead(batteryPin);
    batteryvoltage = (3.3/1023.) * 4.7 * batterysenseValue;
    
    data += currentepochtime;
    data += ",";
  
    addFloatToString(data, batteryvoltage, 4, 2);

  return data;
}

static void addFloatToString(String & str, float val, char width, unsigned char precision)
{
  char buffer[10];
  dtostrf(val, width, precision, buffer);
  str += buffer;
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

void greenred4flash()  // blink the red/green LEDs rapidly a few times at startup
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
