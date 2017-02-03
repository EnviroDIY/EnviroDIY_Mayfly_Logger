#include <Arduino.h>
#include <wire.h>

void setup()
{
  Serial.begin(57600);  //Start the serial connection with the computer (UART-0)

  Serial1.begin(9600);    //this is the Mayfly's default Xbee port (UART-1)

}

void loop()
{

 Serial.println("This is Serial Data printed to the computer");

 delay(500);

 Serial1.print("This is Serial Data sent to the Xbee port");

 delay(5000);
}
