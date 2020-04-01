#include <Arduino.h>

void setup()
{
  Serial.begin(57600);  // Start the serial connection with the computer (UART-0)

  Serial1.begin(9600);  // Start the connection on the Mayfly's Xbee port (UART-1)
}

void loop()
{

 Serial.println("This is Serial Data printed to the computer");

 delay(500);

 Serial1.print("This is Serial Data sent to the Xbee port");

 delay(5000);
}
