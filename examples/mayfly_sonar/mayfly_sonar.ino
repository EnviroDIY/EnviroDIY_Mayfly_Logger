// This Mayfly sketch parses data from MaxSonar serial data and prints it on the serial monitor

// Connect the Maxbotix serial data output pin to Mayfly pin 5
// Provide 3.3v and GND to the Maxbotix sensor power pins

#include <Arduino.h>
#include <wire.h>
#include <SoftwareSerial.h>
SoftwareSerial sonarSerial(5, -1);            //define serial port for recieving data.

int range;

boolean stringComplete = false;

void setup()
{
  Serial.begin(57600);                                      //start serial port for display
  sonarSerial.begin(9600);                                 //start serial port for maxSonar
  pinMode(5, INPUT);

  Serial.println("Mayfly Maxbotix sonar sensor rangefinder example");
  delay(3000);
}

void loop()
{
  range = EZreadSonar();
  if(stringComplete)
  {
    stringComplete = false;                                //reset sringComplete ready for next reading

    Serial.print("Range: ");
    Serial.print(range);
    Serial.println(" mm");

    delay(1000);
  }
}


int EZreadSonar()
{
  int result;
  char inData[5];                                          //char array to read data into
  int index = 0;

  sonarSerial.flush();                                     // Clear cache ready for next reading

  while (stringComplete == false) {
    //Serial.print("reading ");    //debug line

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
    }
  }

  return result;
}
