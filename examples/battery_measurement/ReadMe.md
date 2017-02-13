Example: Battery Measurement on a Mayfly
==============

An example sketch to show people how to measure the voltage of a LiPo battery connected to a Mayfly board.  Most other Arduino boards can't measure their own battery voltage, so it's hard to find other examples on the internet!

### Background
On the Mayfly, analog pin A6 is connected to a resistor divider that measures the voltage of the battery connected to the LiPo jack.  However, if the USB cable or an FTDI adapter is connected to the Mayfly (which is very likely since you’re viewing the data on the Serial Monitor) the measured voltage will be from the USB/FTDI cable and not the LiPo battery.  But this example shows the theory behind measuring and calculating the battery voltage, so this concept can be used in other sketches that are battery powered.


### Tech Details

The 1.47 multiplier in this equation comes from the standard method of measuring the voltage of a voltage divider network.  You can google "voltage divider" or "resistor divider" to learn more.  But basically, the Arduino can't measure anything above the 3.3v operating voltage of the board. So connecting a LiPo battery that can get to 4.2v or more could damage the board if you try to measure it directly without first reducing that voltage to something within the safe range.  So you put 2 resistors in series and measure the voltage in between them.  If the resistors were exactly the same value, then you'd see exactly half of the overall voltage.  But in our case, we only need to reduce it a little, so there's a 4.7M resistor and a 10M resistor.  Measuring the voltage in the middle results in a value about 2/3 of what the overall battery voltage is.  The formula for figuring out that multiplier is (R1+R2)/R1.  So (10+4.7)/10 = 1.47.

### Future (notes from Shannon Hicks)
With the new Mayfly v0.5 board, I made some changes to the way the input battery voltage gets supplied to various parts of the circuit, and also increased the max external battery voltage from 5v to 16v.  But in order to do that, I had to change the values of the resistors in that divider network so that the board could still safely measure a 16v battery but only "see" a max of 3.3v.  So I used a 10M and 2.7M resistor.  Using those values, the multiplier is now 4.7, but it still allows the board to measure the battery voltage with 0.016v resolution.  Previously we had the capability to measure it at 0.005v resolution, but I can't get better resolution AND protect from high battery voltages at the same time.  We always rounded up to 10mv steps anyway, so the new 16mv steps interval won't be much more coarse than what we were doing before.
