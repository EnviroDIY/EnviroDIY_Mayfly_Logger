EnviroDIY Mayfly Data Logger
==============
The [EnviroDIY™ Mayfly Data Logger](http://envirodiy.org/mayfly/) is an Arduino-framework microcontroller board designed specifically for environmental Internet of Things (IoT) applications with the goal to facilitate professional-quality, [do-it-yourself (DIY) environmental science and monitoring](https://www.envirodiy.org).

Mayfly Data Logger can be programmed using standard Aruduino code and community libraries via the [Arduino Integrated Development Environment (IDE) software](https://www.arduino.cc/en/Main/Software) or the more powerful [PlatformIO IDE](https://platformio.org). The Mayfly Data Logger features the ATmega1284p processor, which has 4 times more flash memory for sketches, 8 times more RAM, and almost twice as many input pins as the ATmega328p chip found in the most common Arduino boards. The Mayfly Data Logger also has an integrated solar lipo battery charger, a precise real time clock (RTC), a microSD memory card socket, an XBee radio module socket, and other [hardware features](https://www.envirodiy.org/mayfly/hardware/features/).

This [EnviroDIY_Mayfly_Logger Github repository](https://github.com/EnviroDIY/EnviroDIY_Mayfly_Logger) provides hardware design files and example code sketches, the [EnviroDIY community](https://www.envirodiy.org) provides support via project posts and forums, and the [Mayfly Logger board is available for purchase on on Amazon](https://www.amazon.com/s/ref=nb_sb_noss_1?url=search-alias%3Daps&field-keywords=envirodiy) for user convenience.

<img src="http://363hx939kn9r1sp3hxo0om0z.wpengine.netdna-cdn.com/wp-content/uploads/2016/05/20160508_134514-001.jpg" width="300">

## Getting Started

1. Read [Getting Started With the Mayfly Data Logger](http://envirodiy.org/mayfly/) on the EnviroDIY community website.
2. Purchase an [EnviroDIY Mayfly Data Logger Board](https://www.amazon.com/EnviroDIY-Mayfly-Logger-Arduino-Compatible/dp/B01F9B4WCG) or an [EnviroDIY Mayfly Data Logger Board and Starter Kit](https://www.amazon.com/EnviroDIY-Mayfly-Arduino-Compatible-Starter/dp/B01FCVALDW) on Amazon.com.
3. Install the [Arduino Integrated Development Environment (IDE) application](https://www.arduino.cc/en/Main/Software) on your computer; add "EnviroDIY ATmega Boards" using the Tools > Board > Boards Manager as described in the [Mayfly Data Logger Software](http://envirodiy.org/mayfly/software/) post.
4. Clone or [download](https://github.com/EnviroDIY/EnviroDIY_Mayfly_Logger/archive/master.zip) (and unzip) this entire repository into your Arduino Sketchbook directory, for schematics, and code examples to learn how to program the Mayfly Board.
5. Learn Mayfly basics following our [Mayfly Data Logger Software](http://envirodiy.org/mayfly/software/) post and running the example and workshop code sketches, using the Arduino IDE (from File > Sketchbook), from our [EnviroDIY_Mayfly_Logger > examples](https://github.com/EnviroDIY/EnviroDIY_Mayfly_Logger/tree/master/examples) or [EnviroDIY_Mayfly_Logger > workshop](https://github.com/EnviroDIY/EnviroDIY_Mayfly_Logger/tree/master/workshop) folders.
6. Advance your capabilities to deploy solar-powered environmental monitoring stations with our [LearnEnviroDIY tutorial](https://envirodiy.github.io/LearnEnviroDIY/) to use our [ModularSensors](https://github.com/EnviroDIY/ModularSensors) code library and other [EnviroDIY libraries on GitHub](https://github.com/EnviroDIY) .

## Design & Documentation

We share our EnviroDIY Mayfly logger hardware design files and documentation in this repository's [hardware](https://github.com/EnviroDIY/EnviroDIY_Mayfly_Logger/tree/master/hardware) and [doc](doc) folders, including:
- [Board schematic images](hardware), for versions 0.3, 0.4, 0.5, and 0.5b.
- [EAGLE PCB electronic design files](hardware/Mayfly_v0p5b), for use with the EAGLE electronic design automation (EDA) software (a [free maker edition](https://www.autodesk.com/products/eagle/compare) is available).


## Contribute
Open a [GitHub issue](https://github.com/EnviroDIY/EnviroDIY_Mayfly_Logger/issues) in this repository to suggest and discuss potential changes/additions.

For power contributors, use the GitHub Web application and [GitHub Desktop](https://desktop.github.com) to:

1. Fork our repository into your GitHub account!
2. Create your feature branch
3. Commit your changes
4. Push to your branch
5. Submit a pull request to EnviroDIY!


## License
Software sketches and code are released under the BSD 3-Clause License -- See [LICENSE.md](https://github.com/EnviroDIY/EnviroDIY_Mayfly_Logger/blob/master/LICENSE.md) file for details.

Documentation is licensed as [Creative Commons Attribution-ShareAlike 4.0](https://creativecommons.org/licenses/by-sa/4.0/) (CC-BY-SA) copyright.

Hardware designs shared are released, unless otherwise indicated, under the [CERN Open Hardware License 1.2](https://www.ohwr.org/projects/cernohl/wiki) (CERN_OHL).

## Credits
[EnviroDIY](http://envirodiy.org/)™ is presented by the Stroud Water Research Center, with contributions from a community of enthusiasts sharing do-it-yourself ideas for environmental science and monitoring.

[Shannon Hicks](https://github.com/s-hicks2) is the primary developer of the EnviroDIY Mayfly Data Logger hardware, with input from many [other contributors](https://github.com/EnviroDIY/EnviroDIY_Mayfly_Logger/graphs/contributors).

The Mayfly was developed based on the [SODAQ Mbili board](https://support.sodaq.com/sodaq-one/sodaq-mbili-1284p/) hardware and the [SODAQ Mbili bootloader](https://github.com/SodaqMoja/HardwareMbili) software, designed by Gregory Knauff (GKNFF).

This project has benefited from the support from the following funders:

* William Penn Foundation
* US Environmental Protection Agency (EPA)
* National Science Foundation, awards [EAR-0724971](http://www.nsf.gov/awardsearch/showAward?AWD_ID=0724971), [EAR-1331856](http://www.nsf.gov/awardsearch/showAward?AWD_ID=1331856), [ACI-1339834](http://www.nsf.gov/awardsearch/showAward?AWD_ID=1339834)
* Stroud Water Research Center endowment
