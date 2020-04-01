
/***************************************
 * Turns on an LED on for one second, then off for one second, repeatedly.
 * Introduces use of variables.
 * Includes Serial Monitor readout in setup function.
 *
 original sketch:
 modified 8 May 2014 by Scott Fitzgerald
 modified 2 Sep 2016 by Arturo Guadalupi
 modified 8 Sep 2016 by Colby Newman
 for Mayfly:
 modified 7 Jun 2017 by Beth Fisher github.com/fisherba
 ****************************************/

// Sketches begin with a list of the outside libraries needed to run the sketch.
// They must be in your .../Arduino/libraries directory for the sketch to run.
#include <Arduino.h>

// This variables list is located before the functions and defines the variables for the sketch.
// Variables are useful for assigning information once at the beginning of a sketch,
// because most sketches will need to repeat information like pin locations.
// Notice that the sketch repeats the "delayTime" four times.
// "int" specifies interger data type.

// Try modifying the delay time and LED pin and re-running the sketch.
int delayTime = 1000; //milliseconds, so 1000 = one second

// Assign pin numbers for built-in LEDs.
int LED_BUILTIN_green = 8; //Mayfly has LEDs at pins 8 (green) and 9 (red)
int LED_BUILTIN_red = 9;   //Mayfly has LEDs at pins 8 (green) and 9 (red)

// The "setup" function runs once when you press reset or power the board.
// The "returned value" for this function is "void", which means it doesn't return a value,
// the function simply performs an action.
void setup()
{
    pinMode(LED_BUILTIN_green, OUTPUT); // all you need to set up the LED blink
    pinMode(LED_BUILTIN_red, OUTPUT);   // all you need to set up the LED blink
    Serial.begin(57600);                //establishes data rate for serial transmission
    delay(delayTime);
    Serial.println("Mayfly: Blink demo 1"); //sends a message to the Serial Monitor (click magnifying glass)
    delay(delayTime);
}

// The "loop" function runs over and over again. It performs an action wouthout returning a value,
// so "void" is once again used in the function declaration.
void loop()
{
    digitalWrite(LED_BUILTIN_green, HIGH); // turn the green LED on (HIGH is the voltage level)
    digitalWrite(LED_BUILTIN_red, LOW);    // and turn the red LED off (LOW is the voltage level)
    delay(delayTime);                      // wait for the time specified in delayTime variable
    digitalWrite(LED_BUILTIN_green, LOW);  // now turn the green LED off by making the voltage LOW
    digitalWrite(LED_BUILTIN_red, HIGH);   // and turn the red LED on
    delay(delayTime);                      // wait for the time specified in delayTime variable
}
