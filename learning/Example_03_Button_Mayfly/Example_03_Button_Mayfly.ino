/*
  Button

 Turns on and off a light emitting diode(LED) connected to digital
 pin 8, when pressing the Mayfly pushbutton

 */

 #include <Arduino.h>
 #include <Wire.h>

const int buttonPin = 21;     // the number of the pushbutton pin
const int greenLEDpin = 8;      // the number of the green LED pin

int i = 0;
// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {

  Serial.begin(57600);   // We'll send debugging information via the Serial monitor
  Serial.println("Mayfly Button testing sketch");
  // initialize the LED pin as an output:
  pinMode(greenLEDpin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

}

void loop(){
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    // turn LED on:
    digitalWrite(greenLEDpin, HIGH);
    i++;
    Serial.print("Button!  ");
    Serial.println(i);
    delay(50);
  }
  else {
    // turn LED off:
    digitalWrite(greenLEDpin, LOW);
  }
}
