EnviroDIY Mayfly Data Logger
==============
The [EnviroDIY™ Mayfly Data Logger](http://envirodiy.org/mayfly/) is an Arduino-framework microcontroller board designed specifically for environmental Internet of Things (IoT) applications with the goal to facilitate professional-quality, [do-it-yourself (DIY) environmental science and monitoring](https://www.envirodiy.org).

This [EnviroDIY_Mayfly_Logger Github repository](https://github.com/EnviroDIY/EnviroDIY_Mayfly_Logger) provides hardware design files and example code sketches, the [EnviroDIY community](https://www.envirodiy.org) provides support via project posts and forums, and the [Mayfly Logger board is available for purchase on on Amazon](https://www.amazon.com/s/ref=nb_sb_noss_1?url=search-alias%3Daps&field-keywords=envirodiy) for user convenience.

<img src="https://github.com/EnviroDIY/EnviroDIY_Mayfly_Logger/blob/master/doc/images/mayfly0.3_sideview_right.jpg" width="300" align="right">

## Features:

- ATmega1284p processor
    - 128kB program (flash) memory, 16kB RAM
    - 2 UART, 1 I2C, 1 SPI connection
    - 32 total I/O pins
    - 3.3V, 8MHz operating frequency
- Integrated 6V solar lipo battery charger
- Precise real time clock (RTC, [DS3231](https://datasheets.maximintegrated.com/en/ds/DS3231.pdf)) with separate power supply
- Auxillary 16 bit ADC ([TI ADS1115](http://www.ti.com/lit/ds/symlink/ads1114.pdf))
- FTDI chip for USB communication
- microSD memory card socket
- XBee radio/wifi/cellular module socket
- 6 [Grove](http://wiki.seeedstudio.com/Grove_System/) connectors with access to I2C, auxillary ADC, and digital pins
- 0.5A switchable power supply for sensors at either 3.3V or 5V
- On-board power switch
- Two controllable LED's, red and green
- One programmable push-button
- Option of using external power supply at 5-12V

## Pin-out

| Pin Number[s] | Function             | Access                                 | Notes |
|---------------|----------------------|----------------------------------------|-------|
| D0/D1         | Serial Rx/Tx (UART)  | Connected to FTDI for USB programming  | Also accessible via programming port, 20 pin header, and XBee socket via SJ8/9 |
| D2/D3         | Serial1 Rx/Tx (UART) | XBee socket Din/Dout (pins 2/3)        | Also accessible via 20 pin header, can be disconnected from XBee socket via SJ8/9 |
| D19           | XBee CTS             | XBee socket CTS (pin 12)               | Also accessible via 20 pin header |
| D20           | XBee RTS             | XBee socket RTS (pin 16)               | Also accessible via 20 pin header |
| D23           | XBee DTR             | XBee socket DTR (pin 9)                | Also accessible via 20 pin header |
| A5=D29*       | XBee Assoc or RI     | XBee socket Assoc or RI (pin 15 or 20) | Not connected to XBee socket by default, use SJ7 to connect; Also accessible via 20 pin header |
| D16/D17       | I2C SCL/SDA ("Wire") | Grove connector                        | Also accessible via 20 pin header |
| D13/D14/D15   | SPI MOSI/MISO/SCK    | microSD card slot & SPI header         |  |
| D12           | microSD chip select  | microSD card slot & SPI header         |  |
| D18           | SD card detect OR RTC 32KHz | 20 pin header                   | Not connected to either SD or RTC by default, use SJ6 for RTC or SJ10 for SD, _do not use both_ |
| A7=D31 _or_ D10 | RTC Interrupt      | 20 pin header                          | A7 is connected to RTC interrupt by default; Use SJ1 to disconnect A7 and/or to connect D10 |
| D22           | Switched power/LED4  | Switched power on all Grove ports      | Pin is not directly accessible, only switched power; Cut SJ3 to disconnect LED4 (red) |
| D21           | user push button     | 20 pin header                          | Cut SJ to disconnect button |
| D8            | LED1 (green)         | 20 pin header                          |  |
| D9            | LED2 (red)           | 20 pin header                          |  |
| A6=D30        | Battery measurement  | None                                   | Connected via voltage divider - for v0.4 and newer, use multiplier of 1.27; for v0.3 and older, use multiplier of 1.47 |
| D4-7, D10-11  | General Purpose I/O  | Grove connector; 20 pin header         |  |
| AA0-AA3       | Auxilliary ADC       | Grove connector; 20 pin header         | Connected to _Auxillary_ adc (TI ADS1115), not to Mayfly analog pins |

## Solder Jumper Connections

| Number | Function                        | Default Connection                |
|--------|---------------------------------|-----------------------------------|
| SJ1    | Connect to interrupt from RTC   | Pin A7 connected to RTC interrupt       |
| SJ2    | Disconnect D21 from user button | Pin D21 is connected to button (closed) |
| SJ3    | Disconnect D22 from LED3        | Pin D22 is connected to LED3 (closed)   |
| SJ4    | Disconnect LED4 from solar      | LED4 indicates solar charging status (closed) |
| SJ5*   | 5V Boost enable                 | Boost enabled                                 |
| SJ6**  | Connect D18 to RTC RST/32kHz    | Not connected (open)                          |
| SJ7    | Pin A5 to XBee ASSOC _or_ RI    | Not connected (all pads open)                 |
| SJ8    | XBee Din to TX0/TX1             | XBee Din connected to TX1                     |
| SJ9    | XBee Dout to RX0/RX1            | XBee Dout connected to RX1                    |
| SJ10** | Connect D18 to SD card detect   | Not connected (open)                          |
| SJ11   | USB FTDI programming enable     | FTDI enabled (closed)                         |
| SJ12   | D10 external pull-up            | D10 (INT2) is not pulled up externally (open) |

\* SJ5 has been removed in versions newer than v0.4
** SJ6 and SJ10 should not both be closed at the same time

## Programming


Mayfly Data Logger can be programmed using standard Aruduino code and community libraries via the [Arduino Integrated Development Environment (IDE) software](https://www.arduino.cc/en/Main/Software) or the more powerful [PlatformIO](https://platformio.org) extensions for [Visual Studio Code](https://code.visualstudio.com/) or [Atom](https://atom.io/).

To use the Mayfly with the Arduino IDE you must add "EnviroDIY ATmega Boards" using the Tools > Board > Boards Manager as described in the [Mayfly Data Logger Software](http://envirodiy.org/mayfly/software/) post

No addition set-up is required to use the PlatformIO.
The proper enviroment settings for the Mayfly in a platformio.ini file are:

```ini
[env:mayfly]
platform = atmelavr
board = mayfly
framework = arduino
```

## Additional Resources and Links

- Read the [Getting Started With the Mayfly Data Logger](http://envirodiy.org/mayfly/) page on the EnviroDIY community website for guidance for beginners with the Mayfly, sensor station maintence, and data management.
- Make use of the [The EnviroDIY Mayfly Data l=Logger Forum](https://www.envirodiy.org/forum/mayfly-data-logger/) to ask and answer questions about the Mayfly
- Learn Mayfly basics following our [Mayfly Data Logger Software](http://envirodiy.org/mayfly/software/) post and running the example and workshop code sketches, using the Arduino IDE (from File > Sketchbook), from our [EnviroDIY_Mayfly_Logger > examples](https://github.com/EnviroDIY/EnviroDIY_Mayfly_Logger/tree/master/examples) or [EnviroDIY_Mayfly_Logger > workshop](https://github.com/EnviroDIY/EnviroDIY_Mayfly_Logger/tree/master/workshop) folders.
- Advance your capabilities to deploy solar-powered environmental monitoring stations with our [LearnEnviroDIY tutorial](https://envirodiy.github.io/LearnEnviroDIY/)
- Utilize the [ModularSensors](https://github.com/EnviroDIY/ModularSensors) code library and other [EnviroDIY libraries on GitHub](https://github.com/EnviroDIY) for datalogging built around the Mayfly
- Find and attend a training workshop for do-it-yourself environmental monitoring built around the Mayfly by contacting the Stroud Water Research Center's [citizen science outreach](https://stroudcenter.org/people/bressler/) or [education](https://stroudcenter.org/education/contact/) departments.

## Design Files & Further Documentation

We share our EnviroDIY Mayfly logger hardware design files and documentation in this repository's [Hardware Design Files](hardware) pages and [Documentation](doc) pages.


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
