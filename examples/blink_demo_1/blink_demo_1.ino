
/***************************************
 * Turns on an LED on for one second, then off for one second, repeatedly.
 * Introduces use of variables. 
 * Includes Serial Monitor readout in setup function.
 * 
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
 * 
 original sketch:
 modified 8 May 2014 by Scott Fitzgerald
 modified 2 Sep 2016 by Arturo Guadalupi
 modified 8 Sep 2016 by Colby Newman
 for Mayfly: 
 modified 7 Jun 2017 by Beth Fisher github.com/fisherba
 ****************************************/

//Sketches begin with a list of the outside libraries needed to run the sketch. 
//They must be in your .../Arduino/libraries directory for the sketch to run.
//These are not required for the blink demo, but they are commonly required in sketches. 
//After you run the sketch, try commenting them out and re-run the sketch.
#include <Arduino.h>
#include <Wire.h>


//This variables list is located before the functions and defines the variables for the sketch. 
//Variables are useful for assigning information once at the beginning of a sketch, 
//because most sketches will need to repeat information like pin locations. 
//Notice that the sketch repeats the "delayTime" four times. 
//"int" specifies interger data type.
//Try modifying the delay time and LED pin and re-running the sketch.
int delayTime = 1000;   //milliseconds, so 1000 = one second
int LED_BUILTIN = 9;   //Mayfly has LEDs at pins 8 (green) and 9 (red)


// The "setup" function runs once when you press reset or power the board. 
// The "returned value" for this function is "void", which means it doesn't return a value, 
// the function simply performs an action. 
void setup ()
{
    pinMode(LED_BUILTIN, OUTPUT);  // all you need to set up the LED blink
    Serial.begin(57600);  //establishes data rate for serial transmission
    delay(delayTime);
    Serial.println("Mayfly: Blink demo 1"); //sends a message to the Serial Monitor (click magnifying glass)
    delay(delayTime);
}


// The "loop" function runs over and over again. It performs an action wouthout returning a value, 
// so "void" is once again used in the function declaration. 
void loop ()
{
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(delayTime);                       // wait for the time specified in delayTime variable
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(delayTime);                       // wait for the time specified in delayTime variable
}
