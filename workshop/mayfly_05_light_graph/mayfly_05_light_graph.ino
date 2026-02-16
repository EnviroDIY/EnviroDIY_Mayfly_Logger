// Prints analog light levels in high frequency to serial port
void setup () 
{
    Serial.begin(57600);
  //  Serial.println("EnviroDIY Mayfly v1.0:  Analog Light");
    pinMode(8, OUTPUT);  digitalWrite(8, HIGH);
    pinMode(9, OUTPUT);  digitalWrite(9, HIGH);
      
}

void loop () 
{
    int light = analogRead(A4);             //ALS light sensor is on A4 for Mayfly 1.0 and 1.1
    float lightpercent = light/10.23;       //converts light bits to light percent
  //  Serial.print("Light: "); 
    Serial.println(lightpercent); 

    delay(10);  // number of milliseconds between loops
}
