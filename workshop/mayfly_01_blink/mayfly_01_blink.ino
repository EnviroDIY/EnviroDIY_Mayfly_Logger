//Mayfly LED Blink demo

int i = 1;
int delaytime = 200;

// the setup function runs once when you press reset or power the boar
void setup() 
{
    Serial.begin(57600);
    Serial.println("EnviroDIY Mayfly: Blink demo");  
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);

}

// the loop function runs over and over again forever
void loop() {

  digitalWrite(8, HIGH);
  digitalWrite(9, LOW);
  delay(delaytime);
  digitalWrite(8, LOW);
  digitalWrite(9, HIGH);
  delay(delaytime);
  
  Serial.println(i);
  i++;
}

