Board Features
==============
The Mayfly Data Logger board has many unique features and hardware options. Learn more below!

<img src="https://github.com/EnviroDIY/EnviroDIY_Mayfly_Logger/tree/master/doc/images/doc/images/mayfly0.3_topview_labels3.jpg" width="600">

## Features of the EnviroDIY Mayfly Data Logger

The microUSB port is used to program the Mayfly via your computer.  Once the board has been programmed, you can also supply power to the Mayfly with any microUSB cable or mobile phone charger that provides 5vDC.

The Bee socket is compatible with any module that use the standard Bee connection footprint.  Modules include Xbee radio modules, GPRSBee cell phone board, and other modules for WiFi or Bluetooth communication.

A 6-pin header is available for programming the board via a standard FTDI interface board or programming cable.  Be sure to observe the polarity and connect the FTDI adapter in the right orientation.

Any 6-volt solar panel can be connected to the JST socket for charging a standard 3.7v LiPo battery.  A yellow LED will light up whenever there is sufficient sunlight to charge the battery.

Two LEDS (one green, one red) are connected to pins D8 (green) and D9 (red) for full user control.

Two JST connectors are labeled LiPo (lithium polymer) battery power connections.  Connect a standard 3.7v LiPo battery to either port.  You should only connect one battery to the Mayfly.  The second port is for devices like the GPRSbee that need a direct connection to the battery.  More information about using the GPRSbee will be provided soon.

The DS3231 Real Time Clock chip needs a small battery to keep it’s clock running when the Mayfly isn’t powered by a main battery.  Insert a CR1220 3v lithium battery (with the + side facing up).  Users who don’t want to keep the RTC backed up can omit the battery.

Two 20-pin headers with standard 0.1″ pin spacing provide access to all of the available pins.

A voltage boost circuit provides a 5 volt supply that can be switched on or off by using pin D22.  This pin is also used to turn the “3.3v_Switched” output on or off.

Grove connectors provide easy interface to 4 groups of digital pins. Three of the connectors contain digital pins (4, 5, 6, 7, 10, and 11) and the forth connector is for I2C protocol.  Note:  on board versions 0.3 and 0.4, the I2C connector has the proper pin order found on all Grove I2C ports, but the white silkscreen printing on the board has the SDA and SCL pins reversed.  The pins are correct, only the printing is swapped.  The printing is correct on version 0.5.

Two more Grove connectors interface with the ADS1115 auxiliary 16-bit A/D converter.

The data card socket on the Mayfly accepts standard microSD memory cards.

The pushbutton is connected to digital pin D21.  Users can have their sketch look for a state change on that pin to provide input to the system.

A small 2×4 header allows users to connect the custom Mayfly microSD right-angle adapter board.  This handy accessory allows you to easily access the memory card when the Mayfly is mounted in an enclosure where the usual horizontal slot is inaccessible.  Do not use a memory card in both sockets at the same time.  Users could also build their own custom adapter boards for this header since it breaks out all of the necessary SPI pins.


## New features on version 0.5:
In March 2017, we released version 0.5.  Here are the main difference between 0.5 and the earlier versions:

- Separate 3.3v regulator on the Switched3.3v bus (makes the board more stable when switching loads on and off, and increase current output available to the load with a max output of 500ma)
- New 5v-boost regulator (SC4503 replaces the obsolete NCP1402)
- Onboard pullup resistor (1M ohm) available via jumper SJ12 for connecting D10 to the pullup (useful for using D10 as a hardware interrupt)
- External voltage input range improved – now accepts 4 to 12v.  A small slide switch on the top of the board allows you to select the usual USB/Lipo power option, or you can select an external voltage source to power the board.  To prevent over-voltage damage, the USB port is disconnected from the Mayfly when the switch is set to “Ext”.  If you are using the Mayfly with an external battery and then want to reprogram the Mayfly, you’ll need to slide the switch to “USB/Lipo” in order to program the board.  Then slide it back to “Ext” to power the board via your external source.  If you have a Lipo battery connected to the “Lipo Batt” JST headers, be sure to keep the slide switch in the “USB/Lipo” position or the board will not be powered.
