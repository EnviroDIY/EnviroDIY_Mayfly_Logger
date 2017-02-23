EnviroDIY Mayfly Data Logger
==============
The [EnviroDIY™ Mayfly Data Logger](http://envirodiy.org/mayfly/) is a powerful, user-programmable microprocessor board that is fully compatible with the Arduino IDE software. The goal of the Mayfly Logger project is to enable professional-quality do-it-yourself environmental science and monitoring.

<img src="http://363hx939kn9r1sp3hxo0om0z.wpengine.netdna-cdn.com/wp-content/uploads/2016/05/20160508_134514-001.jpg" width="300">

##Getting Started

1. Read [Getting Started With the Mayfly Data Logger](http://envirodiy.org/mayfly/).
2. Purchase an [EnviroDIY Mayfly Data Logger Board](https://www.amazon.com/EnviroDIY-Mayfly-Logger-Arduino-Compatible/dp/B01F9B4WCG) or an [EnviroDIY Mayfly Data Logger Board and Starter Kit](https://www.amazon.com/EnviroDIY-Mayfly-Arduino-Compatible-Starter/dp/B01FCVALDW) on Amazon.com.
3. Install the [Arduino Integrated Development Environment (IDE) application](https://www.arduino.cc/en/Main/Software) on your computer; add "EnviroDIY ATmega Boards" using the Tools > Board > Boards Manager as described in the [Mayfly Data Logger Software](http://envirodiy.org/mayfly/software/) post.
4. Clone or [download](https://github.com/EnviroDIY/EnviroDIY_Mayfly_Logger/archive/master.zip) (and unzip) this entire repository into your Arduino Sketchbook directory.

##Assumptions Made
This code sketch was written for a Mayfly Data Logger with an xbee wifi radio module (configuration file included - see 'xbee_radio_config.xml') and a second temperature sensor added on pin A5. The sketch and xbee radio are configured to post data to an ODM2 web service that is currently under development and may be subject to changes.

##Configuring WiFi
Included in this repository is an example XBee Radio configuration file. There are several ways to configure this radio to meet your needs, including in-sketch code. The method used in this example is as follows:

Tools used to configure the XBee Radio:

1. [SparkFun Xbee Explorer](https://www.sparkfun.com/products/11812)
2. [Digi Xbee S6B](http://ftp1.digi.com/support/documentation/90002180.pdf)
3. [XCTU Software](https://www.digi.com/support/productdetail?pid=5585)
4. Mini USB cable

Steps:

1. Download and install the drivers and XCTU software on Digi's support page: https://www.digi.com/support/productdetail?pid=5585
2. Connect your XBee radio to the Explorer board, and connect the board to your computer with the USB cable.
3. Open the XCTU program and follow any prompts.
4. (OPTIONAL) Review the documentation found here: http://www.digi.com/resources/documentation/digidocs/90001458-13/default.htm
5. When fully loaded, click on "Add Devices".
6. When you see your XBee radio, select that device and fill in the serial connection settings as needed. Again, follow any additional prompts given.
7. After your device is connected, select it and ensure the Radio Configuration panel is opened.
8. (OPTIONAL) You may load the configuration profile (the example XML configuration file) included, but be aware this will overwrite your current XBee settings.
9. Ensure you are connected to a WiFi hotspot. This can be set by clicking on the configuration option "Active Scan".
10. Review all settings and make sure they are correct, including the current IP address of the intended WebSDL service.
11. Write the settings to your XBee radio.
12. Feel free to test your settings using the built-in "Consoles Working Mode". Note: Newline, null, and other characters may not input into the console correctly. I would recommend using an ASCII to Hex converter and inputting the raw hex into the appropriate section.
13. At this point, unplug the Explorer board from your computer, remove the XBee radio from the board, and insert the radio into your Mayfly board.

##Sensors and Configuration

Sensors:

1. To add a sensor to the sketch, copy and paste the 'SensorTemplateFile.h' and 'SensorTemplateFile.cpp' files.
2. Rename the files to the desired name of your sensor.
3. Open both of your new files and follow the commented instructions.
4. Add your sensor(s) to the 'Config.h' file in sections 1 and 4
5. Rebuild and upload your sketch - no other files need to be updated
    
Config.h:

Config.h aims to consolidate all important configuration details, without requiring a user to make edits to a large and complex file. Through this file, a user can choose sensors, web service details, and board options like baudrate.

##Contribute
Open an [issue](https://github.com/EnviroDIY/EnviroDIY_Mayfly_Logger/issues) to suggest and discuss potential changes/additions.

For power contributors:

1. Fork it!
2. Create your feature branch: `git checkout -b my-new-feature`
3. Commit your changes: `git commit -am 'Add some feature'`
4. Push to the branch: `git push origin my-new-feature`
5. Submit a pull request :D


##License
Software sketches and code are released under the BSD 3-Clause License -- See [LICENSE.md](https://github.com/EnviroDIY/EnviroDIY_Mayfly_Logger/blob/master/LICENSE.md) file for details.

Documentation is licensed as [Creative Commons Attribution-ShareAlike 4.0](https://creativecommons.org/licenses/by-sa/4.0/) (CC-BY-SA) copyright.

Hardware designs shared are released, unless otherwise indicated, under the [CERN Open Hardware License 1.2](http://www.ohwr.org/licenses/cern-ohl/v1.2) (CERN_OHL).

##Credits
[EnviroDIY](http://envirodiy.org/)™ is presented by the Stroud Water Research Center, with contributions from a community of enthusiasts sharing do-it-yourself ideas for environmental science and monitoring.

[S. Hicks](https://github.com/s-hicks2) is the primary developer of the EnviroDIY Mayfly Logger, with input from many [other contributors](https://github.com/EnviroDIY/EnviroDIY_Mayfly_Logger/graphs/contributors).

This project has benefited from the support from the following funders:

* William Penn Foundation
* US Environmental Protection Agency (EPA)
* National Science Foundation, awards [EAR-0724971](http://www.nsf.gov/awardsearch/showAward?AWD_ID=0724971), [EAR-1331856](http://www.nsf.gov/awardsearch/showAward?AWD_ID=1331856), [ACI-1339834](http://www.nsf.gov/awardsearch/showAward?AWD_ID=1339834)
* Stroud Water Research Center endowment
