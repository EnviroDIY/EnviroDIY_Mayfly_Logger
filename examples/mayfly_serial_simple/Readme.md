# A simple serial example

This example program:
- Sends data to the computer via the FTDI chip on Serial
- Sends data to an XBee via Serial 1

## Stepping through the code

Arduino sketches (really c++ programs) begin with a list of the outside libraries needed to run the sketch.
These libraries must be installed on your .../Arduino/libraries directory for the sketch to run.
This example only uses one library, the Arudino framework library (```Arduino.h```).
We use the include that library with the following code:
```cpp
#include <Arduino.h>
```

The "setup" function runs once when you press reset or power the board.
The "returned value" for this function is "void", which means it doesn't return a value, the function simply performs an action.
In this setup function, we are deciding at what speed the board will communicate and printing out text.
```cpp

void setup()
{
  Serial.begin(57600);  // Start the serial connection with the computer (UART-0)

  Serial1.begin(9600);  // Start the connection on the Mayfly's Xbee port (UART-1)
}

```

The "loop" function runs over and over again.
It performs an action wouthout returning a value, so "void" is once again used in the function declaration.
This functio merely prints some text to the serial monitor and to the XBee socket with a short delay in between.
```cpp
void loop()
{

 Serial.println("This is Serial Data printed to the computer");

 delay(500);

 Serial1.print("This is Serial Data sent to the Xbee port");

 delay(5000);
}
```