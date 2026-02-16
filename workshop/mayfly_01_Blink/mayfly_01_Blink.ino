//Mayfly LED Blink demo

int i = 1;      //create an integer with a value of 1
int delaytime = 200;      //time in milliseconds.  1000ms = 1 second

// the setup function runs once when the board is powered
void setup() 
{
    Serial.begin(57600);
    Serial.println("EnviroDIY Mayfly: Blink demo");  
    pinMode(8, OUTPUT); //declares that pin 8 is an output (Green LED)
    pinMode(9, OUTPUT); //declares that pin 9 is an output (Red LED)

}

// the loop function runs over and over again forever
void loop() {
  
  digitalWrite(8, HIGH);  //turns Green LED on.   (High = On)
  digitalWrite(9, LOW);    //turns Red LED off.   (Low = Off)
  delay(delaytime);     // Pause for the amount of time listed above
  digitalWrite(8, LOW);   //turns Green LED off
  digitalWrite(9, HIGH);  //turns Red LED on
  delay(delaytime);
  
  Serial.println(i);      //Displays the number of times code has been run
  i++;        //adds 1 to the current count
}
