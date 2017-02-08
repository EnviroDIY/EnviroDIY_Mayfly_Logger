//Grove magnetic switch test sketch

//Connect the Grove Magnet Switch to the D10-11 Grove socket on the Mayfly
//Optional:  Connect a Grove Buzzer to the D6-7 Grove socket on the Mayfly

#include <Arduino.h>
#include <Wire.h>

#define MAGNECTIC_SWITCH 10
#define buzzer	6
int tips = 0;

void setup()
{
  Serial.begin(57600);
	pinMode(MAGNECTIC_SWITCH, INPUT);
	pinMode(buzzer,OUTPUT);
  pinMode(22, OUTPUT);      // Setting up Pin 22 to provide power to Grove Ports
  digitalWrite(22, HIGH);   // Provide power to D10-11 and D6-7 Grove Ports
}

void loop()
{
	if(isNearMagnet())//if the magnetic switch is near the magnet?
	{
	        digitalWrite(buzzer,HIGH);
                tips++;
                Serial.println(tips);
                delay(500);
                digitalWrite(buzzer,LOW);
	}
	else
	{

	}
}


/*If the magnetic switch is near the magnet, it will return ture, */
/*otherwise it will return false								*/
boolean isNearMagnet()
{
	int sensorValue = digitalRead(MAGNECTIC_SWITCH);
	if(sensorValue == HIGH)//if the sensor value is HIGH?
	{
		return true;//yes,return ture
	}
	else
	{
		return false;//no,return false
	}
}
