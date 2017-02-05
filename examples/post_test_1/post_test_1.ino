#include <Arduino.h>
#include <Wire.h>
#include <Sodaq_WifiBee.h>

//Wifi Credentials
#define SSID "4DsandCounting"
#define PASSWORD "W4171n843"

//Serial Connections
#define SerialMonitor Serial
int SERIAL_BAUD = 57600;  // Serial port BAUD rate

// BEE connections
#define BeeSerial Serial1
int BEE_BAUD = 9600;  // Bee BAUD rate (9600 is default)
int BEE_DTR_PIN = 23;  // Bee DTR Pin (Data Terminal Ready - used for sleep)
int BEE_CTS_PIN = 19;   // Bee CTS Pin (Clear to Send)

//The body can contain any ASCII data, cannot start with "\r\n"
#define TEST_BODY "Test body data..."

Sodaq_WifiBee wifiBee;

void setup() {
  SerialMonitor.begin(SERIAL_BAUD);
  BeeSerial.begin(BEE_BAUD);

  SerialMonitor.println("Device Type: " + String(wifiBee.getDeviceType()));

  // Set up pins for the *bee
  pinMode(BEE_DTR_PIN, OUTPUT);    // Bee socket DTR pin
  digitalWrite(BEE_DTR_PIN, HIGH);   // Setting this high wakes the Bee, low for sleep.

  // wifiBee.init(BeeSerial, -1, BEEDTR, BEECTS, 1024);
  wifiBee.init(BeeSerial, -1, BEE_DTR_PIN, BEE_CTS_PIN, 1024);
  wifiBee.connectionSettings(SSID, "", PASSWORD);

  //This sets the WifiBee to debug mode over Serial
  wifiBee.setDiag(SerialMonitor);

  SerialMonitor.println("------------------------------------");
  SerialMonitor.println("HTTP POST to http://httpbin.org/post");
  SerialMonitor.println("------------------------------------");

  uint16_t code = 0;
  if (wifiBee.HTTPPost("httpbin.org", 80, "/post", "", TEST_BODY, code)) {
    SerialMonitor.println("------------------");
    SerialMonitor.println("Response Code: " + String(code));
    SerialMonitor.println("------------------");

    char buffer[1024];
    size_t bytesRead;
    if (wifiBee.readResponseAscii(buffer, sizeof(buffer), bytesRead)) {
      SerialMonitor.println("--------------");
      SerialMonitor.println("Full Response:");
      SerialMonitor.println("--------------");
      SerialMonitor.print(buffer);

      buffer[0] = '\0';
      code = 0;

      wifiBee.readHTTPResponse(buffer, sizeof(buffer), bytesRead, code);
      SerialMonitor.println("-------------------");
      SerialMonitor.println("HTTP Response Body:");
      SerialMonitor.println("-------------------");
      SerialMonitor.println("------------------");
      SerialMonitor.println("Response Code: " + String(code));
      SerialMonitor.println("------------------");
      SerialMonitor.print(buffer);
    }
  }
}

void loop()
{
}
