#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <SdFat.h>

//Digital pin 12 is the MicroSD slave select pin on the Mayfly
const int8_t SdSsPin = 12;
SdFat SD;

//The data log file
const char *fileName = "DataLog.txt";

//Data header  (these lines get written to the beginning of a file when it's created)
const char *loggerName = "Mayfly microSD Card Tester";
const char *dataHeader = "SampleNumber, Battery_volts";

int sampleinterval = 10;    //time between samples, in seconds

int samplenum = 1;      // declare the variable "samplenum" and start with 1
int batteryPin = A6;    // on the Mayfly board, pin A6 is connected to a resistor divider on the battery input

int batterysenseValue = 0;  // variable to store the value coming from the analogRead function
float batteryvoltage;       // the battery voltage as calculated by the formula below


void setupLogFile()
{
  //Initialise the SD card
  if (!SD.begin(SdSsPin))
  {
    Serial.println("Error: SD card failed to initialise or is missing.");
  }

  //Check if the file already exists
  bool oldFile = SD.exists(fileName);

  //Open the file in write mode
  File logFile = SD.open(fileName, FILE_WRITE);

  //Add header information if the file did not already exist
  if (!oldFile)
  {
    logFile.println(loggerName);
    logFile.println(dataHeader);
  }

  //Close the file to save it
  logFile.close();
}


void logData(String rec)
{
  //Re-open the file
  File logFile = SD.open(fileName, FILE_WRITE);

  //Write the CSV data
  logFile.println(rec);

  //Close the file to save it
  logFile.close();
}

String createDataRecord()
{
  //Create a String type data record in csv format
  //SampleNumber, Battery
  String data = "";
  data += samplenum;           //creates a string called "data", put in the sample number
  data += ",";                 //adds a comma between values
  batterysenseValue = analogRead(batteryPin);         // reads the analog voltage on the batteryPin, reported in bits
  batteryvoltage = (3.3/1023.) * 1.47 * batterysenseValue;      // converts bits into volts (see batterytest sketch for more info)
  data += batteryvoltage;     //adds the battery voltage to the data string
  samplenum++;   //increment the sample number
  return data;
}


void setup()
{
  //Initialise the serial connection
  Serial.begin(57600);

  //Initialise log file
  setupLogFile();

  //Echo the data header to the serial connection
  Serial.println(dataHeader);

}


void loop()
{
  String dataRec = createDataRecord();

  //Save the data record to the log file
  logData(dataRec);

  //Echo the data to the serial connection
  Serial.println(dataRec);

  delay(sampleinterval*1000);   //multiply by 1000 to convert from milliseconds to seconds

}
