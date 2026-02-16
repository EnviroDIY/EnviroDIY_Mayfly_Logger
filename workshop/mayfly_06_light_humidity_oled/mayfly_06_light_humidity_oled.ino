#include <Wire.h>
//#include <Adafruit_SSD1306.h>    
//#include <Adafruit_GFX.h>
#include "Adafruit_SHT4x.h"

Adafruit_SHT4x sht4 = Adafruit_SHT4x();

//#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
//Adafruit_SSD1306 display(128, 32, &Wire, -1);

int State8 = LOW;
int State9 = LOW;

int batteryPin = A6;    // select the input pin for the potentiometer
int batterysenseValue = 0;  // variable to store the value coming from the sensor
float batteryvoltage;
  
void setup () 
{
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    Serial.begin(57600);
    Wire.begin();
  
    Serial.println("EnviroDIY Mayfly v1.0: Onboard sensor demo");   
    sht4.begin();
    sht4.setPrecision(SHT4X_HIGH_PRECISION);
    sht4.setHeater(SHT4X_NO_HEATER);

//  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);  // initialize with the I2C addr 0x3C (for the 128x64)
//  display.clearDisplay();
//  display.setTextSize(1);
//  display.setTextColor(WHITE);
//  display.setCursor(0,0);
//  display.println("Mayfly");
//  display.println("Light/Humidity");
//  display.display();

}

void loop () 
{
    if (State8 == LOW) {    //these 8 lines of code blink the red and green LEDs in alternating fashion for each sample
      State8 = HIGH;
    } else {
      State8 = LOW;
    }
    digitalWrite(8, State8);  
    State9 = !State8;
    digitalWrite(9, State9);

  batterysenseValue = analogRead(batteryPin);    //read the battery voltage 
  batteryvoltage = (3.3/1023.) * 4.7 * batterysenseValue;   //convert bits to volts

  int light = analogRead(A4);    //read the analog light sensor on the Mayfly, which is hardwired to pin A4
  float lightpercent = light/10.23;  //scale it for percent
  
  sensors_event_t humidity, temp;    //set up the sht40 object
  sht4.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data

  Serial.print("Temperature: "); Serial.print(temp.temperature); Serial.print(" degrees C;     ");
  Serial.print("Humidity: "); Serial.print(humidity.relative_humidity); Serial.print("% rH;     ");
  Serial.print("Light: "); Serial.print(lightpercent,1);Serial.print("%;    ");
  Serial.print("Battery: "); Serial.print(batteryvoltage);Serial.println("v;   ");

  //      //the OLED code:  
  //      display.clearDisplay();   //you always have to clear the display each time you want to push new text to it
  //      display.setTextSize(1);
  //      display.setTextColor(WHITE);
  //      display.setCursor(0,0);
  //      //display.print("Temp: "); 
  //      display.print(temp.temperature); display.print((char)247);  display.println("C");
  //      //display.print("Hum: "); 
  //      display.print(humidity.relative_humidity); display.println(" %rH");
  //      display.print("Light: "); display.print(lightpercent,1); display.println("%");
  //      display.print("Battery: "); display.print(batteryvoltage); display.print("v");
  //      display.display();   //you always have to end each push with this line in order to actually display all of the various lines above on the screen 

    delay(1000);
}
