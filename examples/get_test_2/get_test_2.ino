#include <Arduino.h>
#include <Wire.h>
#include <Sodaq_DS3231.h>

// BEE connections
int BEE_BAUD = 9600;  // Bee BAUD rate (9600 is default)
int SERIAL_BAUD = 57600;  // Serial port BAUD rate
int BEE_DTR_PIN = 23;  // Bee DTR Pin (Data Terminal Ready - used for sleep)
int BEE_CTS_PIN = 19;   // Bee CTS Pin (Clear to Send)

// This function returns the datetime from the realtime clock as a string formated for the POST request
String getDateTime(void)
{
  String dateTimeStr;
  //Create a DateTime object from the current time
  DateTime dt(rtc.makeDateTime(rtc.now().getEpoch()));
  //Convert it to a String
  dt.addToString(dateTimeStr);
  return dateTimeStr;
}

// This function generates the GET request
String generatePostRequest()
{
    String request = "GET http://httpbin.org/get\r\n";
    request += "Cache-Control: no-cache\r\n";
    request += "\r\n";
    request += getDateTime();
    request += "\r\n\r\n";

    return request;
}
void setup() {
  // Start the primary serial connection
  Serial.begin(57600);
  // Start the serial connection with the *bee
  Serial1.begin(57600);

  // Set up pins for the *bee
  pinMode(BEE_DTR_PIN, OUTPUT);    // Bee socket DTR pin
  digitalWrite(BEE_DTR_PIN, HIGH);   // Setting this high wakes the Bee, low for sleep.

  // Clear the Bee buffer, then send the request.
  Serial1.flush();
  Serial1.print(generatePostRequest().c_str());
  Serial1.flush();

  // Copy to the main serial port
  Serial.flush();
  Serial.println(" -- Request -- ");
  Serial.print(generatePostRequest().c_str());
  Serial.flush();

  // Add a brief delay for at least the first 12 characters of the HTTP response
  int timeout = 15000;
  while ((timeout > 0) && Serial1.available() < 12)
  {
    // Serial.print(timeout);
    // Serial.print("---");
    // Serial.print(Serial1.available());
    // Serial.print("\n");
    delay(1);
    timeout--;
  }

  // Process the HTTP response
  if (timeout > 0 && Serial1.available() >= 12)
  {
    char response[10];
    char code[4];
    memset(response, '\0', 10);
    memset(code, '\0', 4);

    Serial.println("\n -- Response -- ");
    Serial.print(response);
    Serial.println(code);

  }
  else // Otherwise timeout, no response from server
  {
      Serial.print("HTTP_TIMEOUT\n");
  }
}

void loop()
{
}
