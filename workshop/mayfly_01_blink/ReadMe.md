Workshop Sketch 1: Blink
==============

Lights blink!

###Basic code elements, Lesson 1:

* Comments
  * Anything after a two slashes (//) is ignored by compiler
* Initializing variables
  * Required to assign space in memory for all variables that will change and which variable type is allowed
  * different types include: int, string, float, etc.
  * In the Processing language, variables are permanently associated with a type. Other languages, such as Python, allow for dynamic typing.
  * For full list of types, see [https://www.arduino.cc/en/Reference/HomePage](https://www.arduino.cc/en/Reference/HomePage)
* Setup block
  * Runs once
  * Sets up device for the desired use
    * Tells Mayfly which pins will be used, and in what mode
  * See [https://www.arduino.cc/en/Reference/Setup ](https://www.arduino.cc/en/Reference/Setup)
* Loop block
  * Repeats over and over again
  * Time is recorded in milliseconds
  * See [https://www.arduino.cc/en/Reference/Loop](https://www.arduino.cc/en/Reference/Loop) 
* Variable reassignment:
  * `i = i +1`
  * `i++`


### Interactive Excercise

* Adjust blink rate
  * Change `delaytime` variable, and Upload code again (overwriting last upload)

