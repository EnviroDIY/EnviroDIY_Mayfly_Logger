# Blinking LED's: Version 2

This example program:
- Turns on an LED on for one second, then off for one second, repeatedly.
- Introduces use of variables.

This is another blink demo is designed for first-time users of Arduino and the EnviroDIY Mayfly logger.

The Mayfly has two LED lights attached to digital pins 8 and 9.
Both lights can be controlled using code with the ```pinMode``` and ```digitalWrite``` functions.

## Stepping through the code

Arduino sketches (really c++ programs) begin with a list of the outside libraries needed to run the sketch.
These libraries must be installed on your .../Arduino/libraries directory for the sketch to run.
This example only uses one library, the Arudino framework library (```Arduino.h```).
We use the include that library with the following code:
```cpp
#include <Arduino.h>
```

After including the Arduino framework, we name some "global variables" for the sketch.
This variables list is located before the functions and defines the variables for the sketch.
Variables are useful for assigning information once at the beginning of a sketch, because most sketches will need to repeat information like pin locations.
The text ```int``` before the name of the variable specifies interger data type.

Try modifying the delay time and LED pin and re-running the sketch.
```cpp
int stateGreenLED = LOW;
int pinGreenLED = 8;
int stateRedLED = LOW;
int pinRedLED = 9;

int delayTime = 1000;  // milliseconds
```

The "setup" function runs once when you press reset or power the board.
The "returned value" for this function is "void", which means it doesn't return a value, the function simply performs an action.
```cpp
void setup ()
{
    // set each pin as output to send signals out to the LED
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    // establish the data rate for serial transmission
    Serial.begin(57600);
    delay(1000);
    // send a message to the Serial Monitor
    Serial.println("Mayfly: Blink demo 2");
    delay(1000);
}
```

The "loop" function runs over and over again.
It performs an action wouthout returning a value, so "void" is once again used in the function declaration.
In this function we are having each LED change state (from on to off or vice versa) with every re-run of the loop.
This shows two ways to change the state value.
In the first portion, we check what the state of the green LED was and then set it to be the opposite using the if/else structure.
In the second portion, we simply set the value of the red LED to be "not" or the opposite (represented by the ```!```) of the green LED.
```cpp
void loop ()
{
    if (stateGreenLED == LOW) {
        stateGreenLED = HIGH;
    } else {
        stateGreenLED = LOW;
    }
    digitalWrite(pinGreenLED, stateGreenLED);
    stateRedLED = !stateGreenLED;
    digitalWrite(pinRedLED, stateRedLED);
    delay(delayTime);
}
```

## Notes

This demo assumes you have installed the proper drivers to connect to the Mayfly.
If you need a resource for getting started with Arduino, the tutorial by Adafruit at https://learn.adafruit.com/series/ladyadas-learn-arduino will get you connected and running a blink demo in about an hour.

To run this demo you will need to first connect to the Mayfly by specifying your board under
Tools>Board (select appropriate Mayfly board) and specifying which communication port to use
under Tools>Port.

Once connected to the mayfly, you need to Verify/Compile the sketch by clicking the checkmark at the top, left corner of the Arudino app window. If there is an error, it will appear (in orange) below.
If there are no errors the window color will not change and it will read "Done compiling."
Always compile before uploading a sketch to your Arduino.
Once compiled, click on the right arrow button to upload the sketch to the Mayfly.
If it worked, the LED lights on the board will begin to blink.