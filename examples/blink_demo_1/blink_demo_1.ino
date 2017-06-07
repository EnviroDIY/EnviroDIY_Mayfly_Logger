
/*
 * This is blink demo designed for the EnviroDIY Mayfly logger. The Mayfly has two LED lights 
 * at pins 8 and 9. To run this demo you will need to first connect to the Mayfly by 
 * specifying your board under Tools>Board (select appropriate Mayfly board) and 
 * specifying which communication port to use under Tools>Port. This demo assumes you
 * have installed the proper drivers to connect to the Mayfly. If you need a resource for 
 * getting started with Arduino, the tutorial by Adafruit at 
 * https://learn.adafruit.com/series/ladyadas-learn-arduino
 * will get you connected and running a blink demo in about an hour.
 * 
 * Once connected to the mayfly, you need to Verify/Compile the sketch by clicking the
 * checkmark at the top, left corner of the Arudino app window. If there is an error,
 * it will appear (in orange) below. If there are no errors the window color will not change
 * and it will read "Done compiling." Always compile before uploading a sketch to your Arduino.
 * Once compiled, click on the right arrow button to upload the sketch to the Mayfly. 
 * If it worked, one of the LED lights on the board will begin to blink.  
 */

//Sketches begin with a list of the outside libraries needed to run the sketch. 
//They must be in your .../Arduino/libraries directory for the sketch to run.
//Neither of these are required for the blink demo, but they are commonly required in sketches. 
//After you run the sketch, try commenting them out and re-run the sketch.
#include <Arduino.h>
#include <Wire.h>


//This variables list is located before the "void" sets defines the variables for the sketch. 
//Variables are useful for assigning information once at the beginning of a sketch, 
//because most sketches will need to repeat information like pin locations. 
//Notice that the sketch repeats the "delayTime" four times. 
//"int" specifies interger data type.
//Try modifying the delay time and LED pin and re-running the sketch.
int delayTime = 500;   //milliseconds, so 1000 = one second
int LED_BUILTIN = 9;   //Mayfly has LEDs at pins 8 (green) and 9 (red)


// the "setup" function runs once when you press reset or power the board
void setup ()
{
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(57600);  //establishes data rate for serial transmission
    delay(delayTime);
    Serial.println("Mayfly: Blink demo 1"); //sends a message to the Serial Monitor
    delay(delayTime);
}


// the "loop" function runs over and over again, forever
void loop ()
{
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(delayTime);                       // wait for the time specified in delayTime variable
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(delayTime);                       // wait for the time specified in delayTime variable
}
