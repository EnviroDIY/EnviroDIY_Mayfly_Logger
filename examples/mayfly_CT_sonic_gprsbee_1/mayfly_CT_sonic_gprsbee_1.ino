#include <Wire.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <SPI.h>
#include <SD.h>
#include <SDI12_PCINT3.h>
#include <GPRSbee.h>
//#include <Adafruit_ADS1015.h>

//SODAQ  libraries
#include <RTCTimer.h>
#include <Sodaq_DS3231.h>
#include <Sodaq_PcInt_Mod.h>

#include <SoftwareSerial_PCINT12.h>

const int SonarExcite = 10;
SoftwareSerial_PCINT12 sonarSerial(11, -1);            //define serial port for recieving data, output from maxSonar is inverted requiring true to be set.

String targetURL;
boolean stringComplete = false;
#define APN "apn.konekt.io"

#define READ_DELAY 1

//RTC Timer
RTCTimer timer;

String dataRec = "";
int currentminute;
long currentepochtime = 0;
float boardtemp = 0.0;
int testtimer = 0;   
int testminute = 2;

int batteryPin = A6;    // select the input pin for the potentiometer
int batterysenseValue = 0;  // variable to store the value coming from the sensor
float batteryvoltage;
//Adafruit_ADS1115 ads;     /* Use this for the 16-bit version */

char CTDaddress = '1';      //for one sensor on channel '1' 
float CTDtempC, CTDcond;
int range_mm;

#define DATAPIN 7         // change to the proper pin for sdi-12 data pin
SDI12 mySDI12(DATAPIN); 

int SwitchedPower = 22;    // sensor power is pin 22 on Mayfly

//#define XBEE_SleepPin 23

#define GPRSBEE_PWRPIN  23  //DTR
#define XBEECTS_PIN     19   //CTS

//RTC Interrupt pin
#define RTC_PIN A7
#define RTC_INT_PERIOD EveryMinute

#define SD_SS_PIN 12

//The data log file
#define FILE_NAME "SL0xxLog.txt"

//Data header
#define LOGGERNAME "SL0xx - Mayfly CT & Ultrasonic Logger"
#define DATA_HEADER "DateTime_EST,TZ-Offset,Loggertime,BoardTemp,Battery_V,CT_temp_DegC,CT_cond_dS/m,SonarRange_mm"
     

void setup() 
{
  //Initialise the serial connection
  Serial.begin(57600);
  Serial1.begin(57600);
  sonarSerial.begin(9600);
  rtc.begin();
  delay(100);
  mySDI12.begin(); 
  delay(100);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(SwitchedPower, OUTPUT);
  digitalWrite(SwitchedPower, LOW);
  pinMode(SonarExcite, OUTPUT);
  digitalWrite(SonarExcite, LOW);  //pin 10 is the power pin for the ultrasonic sensor



  pinMode(23, OUTPUT);    // Bee socket DTR pin
  digitalWrite(23, LOW);   // on GPRSbee v6, setting this high turns on the GPRSbee.  leave it high to keep GPRSbee on.  

  gprsbee.init(Serial1, XBEECTS_PIN, GPRSBEE_PWRPIN);
  //Comment out the next line when used with GPRSbee Rev.4
  gprsbee.setPowerSwitchedOnOff(true);
  
//  ads.begin();       //begin adafruit ADS1015

  greenred4flash();   //blink the LEDs to show the board is on

  setupLogFile();

  //Setup timer events
  setupTimer();
  
  //Setup sleep mode
  setupSleep();
  
  //Make first call
  Serial.println("Power On, running: SL07x_mayfly_CT_sonic_gprsbee_2.ino");
  //showTime(getNow());
  
   gprsbee.off();

}

void loop() 
{
   
  //Update the timer 
  timer.update();
  
  if(currentminute % testminute == 0)
     {   //Serial.println("Multiple of x!!!   Initiating sensor reading and logging data to SDcard....");
          
          digitalWrite(8, HIGH);  
          dataRec = createDataRecord();
    
          delay(500);    
          digitalWrite(22, HIGH);
          delay(1000);
          CTMeasurement(CTDaddress);  
          digitalWrite(22, LOW);
          delay(100);
   

          digitalWrite(10, HIGH);
          delay(1000);

          range_mm = SonarRead();

          
          digitalWrite(10, LOW);          

                     
          stringComplete = false; 
    
        
           
          //Save the data record to the log file
          logData(dataRec);
 
   
          //Echo the data to the serial connection
          Serial.println();
          Serial.print("Data Record: ");
          Serial.println(dataRec);      
   
          assembleURL();
   
          //wakeXbee();
          
          delay(500);
        
          digitalWrite(23, HIGH);
          delay(1000);
        
          sendviaGPRS();

          
          delay(1000);

          digitalWrite(23, LOW);    
        
          delay(500);
   
          String dataRec = "";   
     
          digitalWrite(8, LOW);
          delay(500);
          
          testtimer++;      
     }
    if(testtimer >= 5)
      {testminute = 5;
      }
    
    
     //Sleep          
//     Serial.println("Going to sleep");
     systemSleep();
}

void showTime(uint32_t ts)
{
  //Retrieve and display the current date/time
  String dateTime = getDateTime();
  //Serial.println(dateTime);
}

void setupTimer()
{
  
    //Schedule the wakeup every minute
  timer.every(READ_DELAY, showTime);
  
  //Instruct the RTCTimer how to get the current time reading
  timer.setNowCallback(getNow);


}

void wakeISR()
{
  //Leave this blank
}

void setupSleep()
{
  pinMode(RTC_PIN, INPUT_PULLUP);
  PcInt::attachInterrupt(RTC_PIN, wakeISR);

  //Setup the RTC in interrupt mode
  rtc.enableInterrupts(RTC_INT_PERIOD);
  
  //Set the sleep mode
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
}

void systemSleep()
{
  
  //Wait until the serial ports have finished transmitting
  Serial.flush();
  Serial1.flush();
  
  //The next timed interrupt will not be sent until this is cleared
  rtc.clearINTStatus();
    
  //Disable ADC
  ADCSRA &= ~_BV(ADEN);
  
  //Sleep time
  noInterrupts();
  sleep_enable();
  interrupts();
  sleep_cpu();
  sleep_disable();
 
  //Enbale ADC
  ADCSRA |= _BV(ADEN);
  
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

void setupLogFile()
{
  //Initialise the SD card
  if (!SD.begin(SD_SS_PIN))
  {
    Serial.println("Error: SD card failed to initialise or is missing.");
    //Hang
  //  while (true); 
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
  //TimeDate, Loggertime,Temp_DS, Diff1, Diff2, boardtemp
  String data = getDateTime();
  data += ",-5,";   //adds UTC-timezone offset (5 hours is the offset between UTC and EST)
  
    
    rtc.convertTemperature();          //convert current temperature into registers
    boardtemp = rtc.getTemperature(); //Read temperature sensor value
    
    batterysenseValue = analogRead(batteryPin);
    batteryvoltage = (3.3/1023.) * 1.47 * batterysenseValue;
    
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



void CTMeasurement(char i){    //averages 6 readings in this one loop
  CTDtempC = 0;
  CTDcond = 0;
   
  for (int j = 0; j < 6; j++){
  
  String command = ""; 
  command += i;
  command += "M!"; // SDI-12 measurement command format  [address]['M'][!]
  mySDI12.sendCommand(command); 
  delay(500); // wait a while
  mySDI12.flush(); // we don't care about what it sends back

  command = "";
  command += i;
  command += "D0!"; // SDI-12 command to get data [address][D][dataOption][!]
  mySDI12.sendCommand(command);
  delay(500); 
     if(mySDI12.available() > 0){
        float junk = mySDI12.parseFloat();
        float y = mySDI12.parseFloat();
        float z = mySDI12.parseFloat();

      CTDtempC += z;
      CTDcond += y;
     }
  
  mySDI12.flush(); 
     }     // end of averaging loop
     
      CTDtempC /= 6.0;
      CTDcond /= 6.0;
      
      
      dataRec += ",";
      addFloatToString(dataRec, CTDtempC, 3, 1);
      dataRec += ",";      
      addFloatToString(dataRec, CTDcond, 3, 1);
      //dataRec += ",";

}  //end of CTMeasurement




int SonarRead() 
{

  int range_try = 0;
  int result;
  char inData[5];                                          //char array to read data into
  int index = 0;


  sonarSerial.flush();                                     // Clear cache ready for next reading

  while (stringComplete == false) {
   // Serial.print("OK reading! ");    //debug line

      if (sonarSerial.available())
    {
      char rByte = sonarSerial.read();                     //read serial input for "R" to mark start of data
      if(rByte == 'R')
      {
        //Serial.println("rByte set");
        while (index < 4)                                  //read next three character for range from sensor
        {
          if (sonarSerial.available())
          {
            inData[index] = sonarSerial.read(); 
            //Serial.println(inData[index]);               //Debug line

            index++;                                       // Increment where to write next
          }  
        }
        inData[index] = 0x00;                              //add a padding byte at end for atoi() function
      }

      rByte = 0;                                           //reset the rByte ready for next reading

      index = 0;                                           // Reset index ready for next reading
      
      stringComplete = true;                               // Set completion of read to true
      result = atoi(inData);                               // Changes string data into an integer for use
      if (result == 300 && range_try < 20)
        {
        stringComplete = false;
         range_try++;
        }
 
    }
  }

      dataRec += ",";    
      dataRec += result;  
     // addFloatToString(dataRec, result, 4, 1);

  return result;
}


void assembleURL()
{
    targetURL = "";
    targetURL = "http://swrcsensors.dreamhosters.com/portalsonar1.php?";
    targetURL += "LoggerID=SL0xx&Loggertime=";
    targetURL += currentepochtime;
    targetURL += "&CTDtemp=";
    addFloatToString(targetURL, CTDtempC, 3, 1);    //float   
    targetURL += "&CTDcond=";
    addFloatToString(targetURL, CTDcond, 3, 1);    //float  
    targetURL += "&SonarRange=";
    targetURL += range_mm;
    targetURL += "&BoardTemp=";    
    addFloatToString(targetURL, boardtemp, 3, 1);     //float 
    targetURL += "&Battery=";    
    addFloatToString(targetURL, batteryvoltage, 4, 2);     //float 
  
}


//void sendviaXbee() {
//     Serial1.println(targetURL);  
//}

void sendviaGPRS()
{
  char buffer[10];
  if (gprsbee.doHTTPGET(APN, targetURL.c_str(), buffer, sizeof(buffer))) {
  }
}

//void sleepXbee() {
//  delay (1000);
//  pinMode (XBEE_SleepPin,OUTPUT);    // put XBee to sleep
//  digitalWrite(XBEE_SleepPin,HIGH);  // Setting this pin to LOW turns off the pull up resistor, thus saving precious current
//}
//
//void wakeXbee() {
//  pinMode(XBEE_SleepPin,OUTPUT);   // Set the "wake-up pin" to output
//  digitalWrite(XBEE_SleepPin,LOW); // wake-up XBee
//  delay(500); //make sure that XBee is ready
//}
