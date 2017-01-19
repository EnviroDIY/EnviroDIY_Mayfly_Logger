This example works to synchronize the EnviroDIY Mayfly RTC clock chip with NTP or computer time by way of microUSB connection to a computer.

**To synchronize the clock:**

1. Plug in the Mayfly, turn it on, and make sure it is visible to the Arduino IDE.
2. Download sync_clock_PC.ino and then open it in the Arduino IDE.  Upload that sketch to the Mayfly.
3. If you would like to synchronize manually, open the serial port monitor in the IDE.  Send the current unix time preceeded by a T to the Mayfly (ie, T1484241080).  It is best to send a time just a few seconds in advance of the current time because it does take a few seconds for it to initialize.  The current unix time stamp can be found at http://www.unixtimestamp.com/ or http://time.sodaq.net/
4. If you have python available and would like to synchronize automatically, after uploading sync_clock_PC.ino to the Mayfly, download and run sync_clock_PC.py.  This will automatically detect and communicate with the Mayfly's RTC and set the clock to within 1 second of either the local computer time (in UTC) or, if the computer is connected to the internet, the US Network Time Protocol service.

**Requirements**

For manual synchronization:
 * Arduino wire library (generally built into IDE)
 * Sodaq DS3231 library (https://github.com/SodaqMoja/Sodaq_DS3231)

For Automatic synchronization: (conda environment file = environment.yml)
* Python 2.7
* pyserial 2.7
* ntplib 0.3.3
