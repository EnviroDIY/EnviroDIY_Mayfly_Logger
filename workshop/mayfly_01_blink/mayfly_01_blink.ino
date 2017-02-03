//Mayfly LED Blink demo

// Declare variables with a type and initialize with an inital value assignment
int i = 1;
int delaytime = 200;  // time in milliseconds

// The setup function runs once when you press reset or power the board
void setup()           // the void keyword is used in function declarations to indicate that no information will be returned outside the function.
{
    Serial.begin(57600);
    Serial.println("EnviroDIY Mayfly: Blink demo");  
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
}

// The loop function runs over and over again forever
void loop() {

  digitalWrite(8, HIGH);
  digitalWrite(9, LOW);
  delay(delaytime);
  digitalWrite(8, LOW);
  digitalWrite(9, HIGH);
  delay(delaytime);
  
  Serial.println(i);
  i = i+1;            // Increment the value of i by +1, or reassign a new value to i that is 1 greater than the previous value
}

