# Logging to data.EnviroDIY.org via XBee WiFi or GPRSBee

This sketch is an simple example of sending data to the [EnviroDIY data portal](http://data.envirodiy.org/) via either [Digi XBee WiFi  S6B](https://www.digi.com/support/productdetail?pid=5585) or [Sodaq GPRSBee](https://shop.sodaq.com/en/gprsbee.html) radio modules.  This example will only send data from the temperature sensor built into the Mayfly's real time clock and the Mayfly's current battery or power supply voltage.  As is, it will send data to a dummy site labeled "srgd_desk", which is Sara Damiano's desk at the Stroud Water Research Center. Therefore PLEASE FORK AND MODIFY FOR YOUR SENSOR STATION.

## Registering at the EnviroDIY Data Portal

1. Navigate to http://data.envirodiy.org/ and click the "Log In" button on the upper right hand corner.
2. If you do not already have an account, select the option to "Sign Up."  Fill in all of your account information and then click "Register User".
3. Once you are logged in, click on "My Sites" in the upper left.  Then click the blue button on the right to "Register a New Site".
4. Fill in the information about your site.  You can either type in the latitude and longitude information or zoom in and pick a point on the map to have the data automatically filled in.
5. Click the "+" next to the word "Sensors" to add a new sensor.
6. Choose your equipment model as "EnviroDIY Mayfly."  Pick a measured variable of "Temperature", unit of "degC", and "Sampled medium: of "Air". Then click the "Add New Sensor" button.
7. Repeat step 6 to add a second identical senor.  (Or, as soon as it's available, select Mayfly Battery Voltage instead!)
8. After all of your sensors are registered, click "Register Site."


## Programming your Mayfly with the Arduino IDE
1. If you have not already done so, follow the [Mayfly Data Logger Software](https://envirodiy.org/mayfly/software/) instructions to install the Arduino IDE and set it up to connect to your Mayfly.
2. Download logging_to_EnviroDIY.ino and place it in a folder named "logging_to_EnviroDIY" in your main Arduino directory.
3. Download the file https://github.com/EnviroDIY/Libraries/raw/master/libraries.zip from the EnviroDIY Libraries GitHub repository.  Decompress the zip file into your Arduino libraries folder.  (See "Manual Installation" in the [Arduino Library Guide](https://www.arduino.cc/en/Guide/Libraries#toc5) for more details.)
4. Open the Arduino.cc software.
5. Configure radio module:
  * If using a WiFiBee, connect it to a WiFi network using the instructions below for "Configuring WiFi."  
  * If using a GPRSBee, put a paid sim card into the slot on the GPRSBee.  Connect a battery to both the Mayfly and the GPRSBee.  (The GPRSBee cannot draw power directly from the microUSB port like the WiFiBee can.)
6. After plugging your WiFiBee or GPRSBee into the socket on your Mayfly, connect your Mayfly to your computer. Turn the Mayfly on using the physical switch on the board.  Make sure the Mayfly is visible to the Arduino Software.
7. Open logging_to_EnviroDIY_wifi.ino from wherever you saved it.  Scroll to line 53 of the sketch.  You should see a string "REGISTRATION_TOKEN = " followed by a string of letters and numbers.  Look back at your site page on data.envirodiy.org.  Find the Registration Token on your site page.  Copy and paste that value into the sketch, replacing the values currently there.  Be careful to keep the value between quotation marks.
8.  Continue to copy and paste the values of the Sampling Feature UUID and both result UUIDs into the corresponding lines in the sketch.
9. If using a GPRSBee, find the line beginning with "const char* APN = " and fill in the proper access point name for your sim card between the quotation marks.
10.  Save the sketch and upload it to the Mayfly by clicking the "Upload" button in the upper left of the Arduino IDE.  Wait for it to finish uploading.
11. Open the Serial Monitor in the Arduino IDE by clicking the button on the upper right.  This will give you a view of the data being sent.  You should see a header, a clump of data between brackets, and, hopefully a message that the data was sent sucessfully.
12.  Refresh your site page on data.envirodiy.org to verify that the data appears there.  If you are not successfully sending data, first verify your UUIDs have been copied correctly.  Next verify your wireless settings, particularly the Destination IP address.  Also ensure that you have good signal for the wireless or 2G network.


## Configuring WiFi
The most difficult part of getting this to example to work is properly configuring the WiFi bee.  Configuring and programming the WiFiBee is most easily done independent of the Mayfly.

Materials used:

- Digi XBee S6B - http://ftp1.digi.com/support/documentation/90002180.pdf
- XCTU Software - https://www.digi.com/support/productdetail?pid=5585
- SparkFun Xbee Explorer - https://www.sparkfun.com/products/11812
- Mini USB cable - Note: this is NOT same as the microUSB cable used by many android phones!

### Steps:
1. Download and install the drivers and XCTU software on Digi's support page: https://www.digi.com/support/productdetail?pid=5585.
2. Connect your Bee radio to the Explorer board, and connect the board to your computer with the miniUSB cable.
3. Open the XCTU program and follow any prompts.
4. When fully loaded, click on "Add Devices".
5. When you see your Bee radio, select that device and fill in the serial connection settings to talk to that Bee - Baud Rate: 9600, Data Bits: 8, Parity: None, Stop Bits: 1, Flow Control: None. Again, follow any additional prompts given.  After your device is connected, select it and ensure the Radio Configuration panel is opened.
6. Connect to a WiFi hotspot by clicking on the configuration option "Active Scan".  Once your desired hotspot is found by the scan, connect to it by entering the proper security infomation.  Wait for the connection to be properly established.
- If you would like to connect to a WiFi network that you know settings for but is not currently in range, instead of using the "Activate Scan" button, manually enter your SSID, Encryption, Passphrase, IP Addressing Mode, etc.  Click the write button to send these settings to the XBee.
7. After connecting to WiFi, find these settings and set them to the proper values for the EnviroDIY Data Portal.  The other settings should be left to default or will be set on connecting to your Wifi.
  - AH - Network type: _Infrastructure [2]_
  - IP - IP Protocol: _TCP [1]_
  - DL - Destination IP Address: _129.123.9.68_
  - DE - Destination Port: _50_
  - AP - API Enable: _Transparent Mode [0]_
8. (OPTIONAL) If you would prefer, you may load the configuration profile (xbee_radio_config.xml) included, but be aware this will overwrite your current Bee settings.  After loading the profile, click the write button.  Then you will need to repeat step 6 and reconnect to your WiFi.
9. Review all settings and make sure they are correct, including the current IP address of the intended WebSDL service.
10. Write the settings to your Bee radio by clicking the "Write" button.
11. At this point, unplug the Explorer board from your computer, remove the Bee radio from the board, and insert the WiFiBee into your MayFly board.  Align the WiFiBee so it matches the outline of the Bee on the Mayfly board.


## Other Notes
It is important to understand two basic limitations of the WiFiBee:

1. It can only connect to the single WiFi hotspot it has been programmed for.  
2. It can only send and recieve data from one IP address/port.

This means that if you program your XBee while sitting at your local coffee shop, it will only be able to use that shop's WiFi until it has been reprogrammed.  This also means that regardless of what website URL you program into your sketch, the only sites the WiFiBee will actually be able to access are those hosted on the server at the destination IP address until the Bee has been reprogrammed.  This sketch calls the EnviroDIY Data Portal.  The WiFiBee can only make a connection to that portal if the destination IP address programmed into it is the IP address of that server.  If you later want to use your WiFiBee to access Google's home page, you will need to reprogram it with Google's IP address.  Both of these limitations could potentially be overcome using a complex enough sketch on the Mayfly, but that is far, far beyond the scope of the example.

## Notes on GPRSBee, 2G vs. 3G
If using a GPRSBee, it is important to remember that it only functions on 2G.  The service areas for 2G are NOT the same as those for 3G or 4G, so your cell phone may see great service even when the GPRSBee sees none.  As of January 1, 2017, AT&T has shut down all 2G service in the United States.  
