This example works to synchronize the EnviroDIY Mayfly RTC clock chip with NTP or computer time by way of microUSB connection to a computer.

**To synchronize the clock:**

1. Ensure that you have the Sodaq library for the [DS3231](https://github.com/SodaqMoja/Sodaq_DS3231) available on your system.
2. Plug in the Mayfly, turn it on with the switch on the board, and make sure it is visible to your system.
3. Download sync_clock_PC.ino and upload it to the Mayfly.
4. Verify on the serial port monitor that your Mayfly is outputting the current date and time.  (Set the serial port baudrate to 9600.)
5. To synchronize manually:  Send the current unix time preceeded by a T to the Mayfly over the serial port (ie, T1484241080).  It is best to send a time just a few seconds in advance of the current time because it does take a few seconds for it to initialize.  The current unix time stamp can be found at http://www.unixtimestamp.com/ or http://time.sodaq.net/
5. To synchronize automatically:  After uploading sync_clock_PC.ino to the Mayfly, close the serial port monitor.  Download and run sync_clock_PC.exe.  This will automatically detect and communicate with the Mayfly's RTC and set the clock to within 1 second of either the local computer time or, if the computer is connected to the internet, the US Network Time Protocol service.  The time will be set in **_UTC_**, not whatever the local timezone is.

**Requirements**

 * Arduino wire library (generally built into IDE)
 * Sodaq DS3231 library (https://github.com/SodaqMoja/Sodaq_DS3231)
