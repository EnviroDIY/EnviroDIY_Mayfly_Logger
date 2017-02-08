This sketch is an simple example of sending data to the [EnviroDIY data portal](http://data.envirodiy.org/) via a [Digi WiFi XBee S6B](https://www.digi.com/support/productdetail?pid=5585).  This example will only send data from the temperature sensor built into the Mayfly's real time clock and the Mayfly's current battery or power supply voltage.  As is, it will send data to a dummy site labeled "srgd_desk", which is Sara Damiano's desk at the Stroud Water Research Center.


##Configuring WiFi
The most difficult part of getting this to example to work is properly configuring the WiFi bee.  Configuring and programming the WiFiBee is most easily done independent of the Mayfly.
Materials used:
    Digi XBee S6B - http://ftp1.digi.com/support/documentation/90002180.pdf
    XCTU Software - https://www.digi.com/support/productdetail?pid=5585
    SparkFun Xbee Explorer - https://www.sparkfun.com/products/11812
    Mini USB cable - Note: this is NOT same as the microUSB cable used by many android phones!
Steps:
    1. Download and install the drivers and XCTU software on Digi's support page: https://www.digi.com/support/productdetail?pid=5585.  While you are there you can also review the module [documetation](http://www.digi.com/resources/documentation/digidocs/90001458-13/default.htm).
    2. Connect your Bee radio to the Explorer board, and connect the board to your computer with the USB cable.
    3. Open the XCTU program and follow any prompts.
    4. When fully loaded, click on "Add Devices".
    5. When you see your Bee radio, select that device and fill in the serial connection settings - Baud Rate: 9600, Data Bits: 8, Parity: None, Stop Bits: 1, Flow Control: None. Again, follow any additional prompts given.  After your device is connected, select it and ensure the Radio Configuration panel is opened.
    6. Connect to a WiFi hotspot by clicking on the configuration option "Active Scan".  Once your desired hotspot is found by the scan, connect to it by entering the proper security infomation.  Wait for the connection to be properly established.
      - If you would like to connect to a WiFi network that you know settings for but is not currently in range, instead of using the "Activate Scan" button, manually enter your SSID, Encryption, Passphrase, IP Addressing Mode, etc.  Click the write button to send these settings to the XBee.
    7. After connecting to WiFi, find these settings and set them to the proper settings for the EnviroDIY Data Portal.  The other settings should be left to default or will be set on connecting to your Wifi.
      - AH - Network type: Infrastructure [2]
      - IP - IP Protocol: TCP [1]
      - DL - Destination IP Address: 129.123.9.68
      - DE - Destination Port: 50
      - AP - API Enable: Transparent Mode [0]
    8. (OPTIONAL) If you would prefer, you may load the configuration profile (xbee_radio_config.xml) included, but be aware this will overwrite your current Bee settings.  After loading the profile, click the write button.  Then you will need to repeat step 6 and reconnect to your WiFi.
    9. Review all settings and make sure they are correct, including the current IP address of the intended WebSDL service.
    10. Write the settings to your Bee radio by clicking the "Write" button.
    11. At this point, unplug the Explorer board from your computer, remove the Bee radio from the board, and insert the radio into your MayFly board.

  It is important to understand two basic limitations of the WiFiBee:  1 - It can only connect to the single WiFi hotspot it has been programmed for.  2 - It can only send and recieve data from one IP address/port.  This means that if you program your XBee while sitting at your local coffee shop, it will only be able to use that shop's WiFi until it has been reprogrammed.  This also means that regardless of what website URL you program into your sketch, the only sites the WiFiBee will actually be able to access are those hosted on the server at the destination IP address until the Bee has been reprogrammed.  This sketch calls the EnviroDIY Data Portal.  The WiFiBee can only make a connection to that portal if the destination IP address programmed into it is the IP address of that server.  If you later want to use your WiFiBee to access Google's home page, you will need to reprogram it with Google's IP address.  Both of these limitations could potentially be overcome using a complex enough sketch on the Mayfly, but that is far, far beyond the scope of the example.
