
//TMP36 Analog Temperature Sensor

// Connect the "Mayfly Grove Analog Sensor Board" to the Mayfly's 20-pin Analog Pin header
// Plug the TMP36 temperature sensor into the 4-pin header labeled "TMP36"

int sensorPin = 1;   //the analog pin the TMP36's Vout (sense) pin is connected to
                        //the resolution is 10 mV / degree centigrade with a
                        //500 mV offset to allow for negative temperatures
 
/*
 * setup() - this function runs once when you turn your Arduino on
 * We initialize the serial connection with the computer
 */
void setup()
{
  Serial.begin(57600);  //Start the serial connection with the computer
                       //to view the result open the serial monitor             
}
 
void loop()                     // run over and over again
{
 //getting the voltage reading from the temperature sensor
 int reading = analogRead(sensorPin);
 
 
 // converting that reading to voltage, for 3.3v arduino use 3.3, use 5.0 for Uno boards
 float voltage = reading * 3.3;
 voltage /= 1024;   //convert bits to volts
 
 // print out the voltage
 Serial.print(voltage); Serial.println(" volts");
 
 // now print out the temperature
 float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
                                               //to degrees ((volatge - 500mV) times 100)
 Serial.print(temperatureC, 1); Serial.println(" degrees C"); // Printing V to 1 figures right of decimal, default is 2 figs

 
 // now convert to Fahrenheight
 float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
 Serial.print(temperatureF); Serial.println(" degrees F");
 Serial.println();
 delay(1000);                                     //waiting a second
}
