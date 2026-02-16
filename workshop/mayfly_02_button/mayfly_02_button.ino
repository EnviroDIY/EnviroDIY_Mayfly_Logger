
//  Mayfly Button demo
 
// Turns on and off a light emitting diode(LED) connected to digital  
// pin 8, when pressing the Mayfly pushbutton


const int buttonPin = 21;     // the pin number of the pushbutton 
const int greenLEDpin = 8;    // the pin number of the green LED 

int i = 0;                    // variable for incrementing
int buttonState = 0;          // variable for reading the pushbutton status

void setup() {
  
  Serial.begin(57600);   // Sends debugging information to the Serial monitor
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
