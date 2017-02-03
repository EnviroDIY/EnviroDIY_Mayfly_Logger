#include <Arduino.h>
#include <wire.h>

int batteryPin = A6;    // on the Mayfly board, pin A6 is connected to a resistor divider on the battery input

int batterysenseValue = 0;  // variable to store the value coming from the analogRead function
float batteryvoltage;       // the battery voltage as calculated by the formula below

void setup() {
  pinMode(8, OUTPUT);         // declare the Green ledPin as an OUTPUT
  digitalWrite(8, HIGH);      // turn the Green ledPin on
  Serial.begin(57600);
}

void loop() {
  // read the voltage value from the battery input resistor divider:
  batterysenseValue = analogRead(batteryPin);

  Serial.print("analogValue= ");
  Serial.print(batterysenseValue);
  Serial.print(" bits;    ");

  batteryvoltage = (3.3/1023.) * 1.47 * batterysenseValue;
  Serial.print("Voltage: ");
  Serial.print(batteryvoltage);
  Serial.println(" volts");
  delay(1000);

}
