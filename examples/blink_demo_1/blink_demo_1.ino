int delayTime = 500;   //milliseconds

void setup () 
{
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    Serial.begin(57600);
    delay(1000);
    Serial.println("Mayfly: Blink demo 1");
    delay(1000);       
}

void loop () 
{
    digitalWrite(8, HIGH);
    digitalWrite(9, HIGH); 

    delay(delayTime);

    digitalWrite(8, LOW);
    digitalWrite(9, LOW);

    delay(delayTime);
}
