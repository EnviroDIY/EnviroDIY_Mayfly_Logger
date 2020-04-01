# Blinking LED's: Version 1

This example program:
- Turns on an LED on for one second, then off for one second, repeatedly.
- Introduces use of variables.
- Includes Serial Monitor readout in setup function.

This is blink demo is designed for first-time users of Arduino and the EnviroDIY Mayfly logger.

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
Notice that the sketch re-uses the value ```delayTime``` four times.
The text ```int``` before the name of the variable specifies interger data type.

Try modifying the delay time and LED pin and re-running the sketch.
```cpp
int delayTime = 1000;   // milliseconds, so 1000 = one second
```

Assign pin numbers for built-in LEDs.
```cpp
int LED_BUILTIN_green = 8;  // Mayfly has a green LED connected to digital pin 8
int LED_BUILTIN_red = 9;    // Mayfly has a red LED connected to digital pin 9
```


The "setup" function runs once when you press reset or power the board.
The "returned value" for this function is "void", which means it doesn't return a value, the function simply performs an action.
```cpp
void setup ()
{
    pinMode(LED_BUILTIN_green, OUTPUT);  // lets the board know we want to use the pin as output to turn on the LED
    pinMode(LED_BUILTIN_red, OUTPUT);    // all you need to set up the LED blink
    delay(delayTime);
}
```

The "loop" function runs over and over again.
It performs an action wouthout returning a value, so "void" is once again used in the function declaration.
```cpp
void loop ()
{
    digitalWrite(LED_BUILTIN_green, HIGH);  // turn the LED on (HIGH is the voltage level)
    digitalWrite(LED_BUILTIN_red, LOW);     // turn the LED on (HIGH is the voltage level)
    delay(delayTime);                       // wait for the time specified in delayTime variable
    digitalWrite(LED_BUILTIN_green, LOW);   // turn the LED off by making the voltage LOW
    digitalWrite(LED_BUILTIN_red, HIGH);    // turn the LED on (HIGH is the voltage level)
    delay(delayTime);                       // wait for the time specified in delayTime variable
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
If it worked, one of the LED lights on the board will begin to blink.

## Credits

Original sketch:
- modified 8 May 2014 by Scott Fitzgerald
- modified 2 Sep 2016 by Arturo Guadalupi
- modified 8 Sep 2016 by Colby Newman for Mayfly:
- modified 7 Jun 2017 by Beth Fisher github.com/fisherba