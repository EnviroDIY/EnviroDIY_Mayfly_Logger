Jumper settings
==============
On the back of the Mayfly v0.5b board, there are 12 user-configurable solder jumpers to allow the user to configure the board with different options.


## Jumpers on back of Mayfly board

![Mayfly v0.5b Bottom View](https://github.com/EnviroDIY/EnviroDIY_Mayfly_Logger/blob/master/doc/images/mayfly0.5b_bottomview.jpg)

On the back of the Mayfly v0.5b board, there are 12 user-configurable solder jumpers.

SJ1:  Connects the RTC Square-Wave Interrupt line to pin A7 (or D10 on board v0.4). This allows the RTC to wake up the Mayfly from sleep mode. Default position is connected to A7.  Cut the trace to free up pin A7 for something else. Add solder bridge to connect center pad to D10 if you’d like to use the D10 hardware interrupt.

SJ2:  Connects the pushbutton to pin D21.  Default position is ON.  Cut the trace to free up D21.

SJ3:  Enables the red LED to be lit when the switched 3.3v line is turned on.  Default position is ON.  Cut the trace to disable the LED.

SJ4:  Enables the yellow LED that is lit whenever the lipo battery is being charged.  Default position is ON.  Cut the trace to disable the LED.

SJ5:  Enables the 5v boost circuitry.  Default position is ENABLED.  Cut the trace and solder the middle pad to the DISABLE pad to deactivate the 5v boost circuit.  Note:  This jumper has been removed on board version 0.5 and later

SJ6:  Connects the RTC Reset or 32khz signal to pin D18.  Default position is NONE.  Solder the middle pad to only one other pad if needed.
Note:  SJ6 and SJ10 are both used for configuring inputs to D18.  Do not use SJ10 if you select a setting for SJ6.

SJ7:  Connects the Bee socket Ring Indicator or Associate lines to A5.  Default position is NONE.

SJ8:  Connects the Bee socket TX signal to Mayfly TXD0 (UART 0) or TXD1 (UART 1).  Default position is TXD1

SJ9:  Connects the Bee socket RX signal to Mayfly RXD0 (UART 0) or RXD1 (UART 1).  Default position is RXD1

SJ10:  Connects the microSD socket’s Card Detect line to D18.  Default position is OFF.  Solder this jumper together if you want to be able to sense a memory card in either the horizontal socket or the vertical adapter board.
Note:  SJ6 and SJ10 are both used for configuring inputs to D18.  Do not use SJ6 if you select a setting for SJ10.

SJ11:  Controls power to the FT232RL USB chip.  Default position is ON.  Cut this trace if you do not want to use the USB port for programming.

SJ12:  *Only on board v0.5 and later*, this jumper allows you to easily connect the onboard 1M-ohm pullup resistor to D10.  Default position is OFF. Add a solder bridge if you want to connect D10 to the onboard pullup resistor.  This is handy for using D10 as a hardware interrupt.  DO NOT use this jumper if you have modified SJ1 to use D10 as the RTC interrupt.  Also, DO NOT use this jumper if you are using D10 for any other type of input besides something that needs the pullup.

SJ13:  *Only on board v0.5b and later*, this jumper allows you to select the power source for the Bee socket’s Vcc pin (Bee pin 1). The default setting is that Bee is powered by the Mayfly’s main 3.3v Vcc supply.  By cutting the SJ13 trace and soldering the middle pad to the LiPo setting, you can connect the Bee Vcc pin directly to the Mayfly’s LiPo battery input.


## Jumpers on Grove Ports

![Mayfly v0.3 Grove Ports](https://github.com/EnviroDIY/EnviroDIY_Mayfly_Logger/blob/master/doc/images/mayfly_groveports.jpg)

On the front of the board, there are 4 small 3-pin headers next to the Grove connectors.  These headers allow you to select what voltage signal goes on the V line of the Grove connector.

The 2 Grove connectors labeled “I2C” and “D4-5” can be powered by either 3.3v constantly or the switchable 3.3v signal (controlled by D22).

The 2 Grove connectors labeled “D6-7” and “D10-11”  can be powered by either the switchable 3.3v line or the switchable 5v line (both controlled by D22)

The 2 Grove connectors for the Auxiliary Analog converter can be powered by either the switchable 3.3v line or the switchable 5v line (both controlled by D22)

These are 2.0mm pitch jumpers, so a standard 0.1-inch jumper will not fit securely.
