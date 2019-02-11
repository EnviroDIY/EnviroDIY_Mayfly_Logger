Workshop Sketch 6: Humidity & Temperature
==============

Connect the Grove Digital Humidity and Temperature (DHT) board to D10-11 Grove connector on the Mayfly.

###Basic code elements, Lesson 2:

* Include Library
  * `#include` is used to include outside libraries in your sketch. This gives the programmer access to a large group of standard C libraries (groups of pre-made functions), and also libraries written especially for Arduino.
  * More information is availble at [https://www.arduino.cc/en/Reference/Include](https://www.arduino.cc/en/Reference/Include)
* Sensor documentation
  * It is ofen helpful to find and read documenation for sensors or periferal devices. For this Grove Digital Humidity and Temperature sensor board, see [http://wiki.seeed.cc/Grove-TemperatureAndHumidity_Sensor/](http://wiki.seeed.cc/Grove-TemperatureAndHumidity_Sensor/)

### Mayfly Board Lessons

* Power to Grove Ports: For any Grove sensor connected to the Grove ports on the Mayfly, sketches should make sure to have the following 2 lines in the "setup" block near the top of the sketch:

```
pinMode(22,OUTPUT);		// Setting up Pin 22 to provide power to Grove Ports
digitalWrite(22,HIGH);	// Provide power to D10-11 and D6-7 Grove Ports
```

  * These lines are specific to the Mayfly board and won't be found on any other sample sketch that you'll find online for dealing with Grove sensors except for our workshop sketches.  Without those lines, the D10-11 and D6-7 Grove ports on the Mayfly will not have power on the "POWER" pin of the connector, so the  Grove sensor won't be on.  The sketch will compile fine, but you won't get any data from the sensor because it's not powered.  A small red LED in the lower left corner of the Mayfly will turn on whenever the switch power (D22) is turned on.
* This also applies to the two Aux Analog Grove connectors as well. You can use the D4-5 or I2C Grove connectors if you want something to be constantly power, assuming you have the jumper in the correct position ("3.3v" and not "3.3vSwitched").  See the bottom of this page for more information about the Grove jumpers: [http://envirodiy.org/mayfly/hardware/jumper-settings/](http://envirodiy.org/mayfly/hardware/jumper-settings/).


### Interactive Excercise

* Serial Monitor
  * Click on Serial Monitor button to open a separate window

  

