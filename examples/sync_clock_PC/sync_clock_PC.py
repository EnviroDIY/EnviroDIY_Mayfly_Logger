# -*- coding: utf-8 -*-

"""
Created by Sara Geleskie Damiano on 1/9/2017 at 2:24 PM

__author__ = 'Sara Geleskie Damiano'
__contact__ = 'sdamiano@stroudcenter.org'


"""

# import the serial library to talk to the arduino
import serial
import serial.tools.list_ports
import warnings

# import libraries for timezones
import time
import datetime
import ntplib
from tzlocal import get_localzone


# get local timezone
local_tz = get_localzone()


def get_mayfly_time():
    # A helper function to get the current time from a Mayfly running sync_clock_PC.ino
    # Returns the current time as a time-zone aware python date-time object
    mayfly_time_str = mayfly.readline()
    # print mayfly_time_str
    # Convert into a time-zone aware python datetime value
    mayfly_rtc = int(mayfly_time_str.split("(")[1].split(")")[0])
    mayfly_rtc_unaware = datetime.datetime.utcfromtimestamp(mayfly_rtc)
    mayfly_rtc_aware = local_tz.localize(mayfly_rtc_unaware, is_dst=False)
    return mayfly_rtc, mayfly_rtc_aware


def get_pc_time(notifications=False):
    # A helper function to get the current time of either the PC or the US national time protocol servers
    # Checks for internet connection and if that is available returns the US NTP time, otherwise it
    # returns the local PC clock time.
    # Returns the current time (localized to the current PC) as a unix timestamp

    try:
        c = ntplib.NTPClient()
        response = c.request('us.pool.ntp.org', version=3)
        utc_unix_time = response.tx_time
        if notifications:
            print "Using time from Network Time Protocol server us.pool.ntp.org"
    except:
        ts_utc = datetime.datetime.utcnow()
        utc_unix_time = (ts_utc - datetime.datetime(1970, 1, 1)).total_seconds()
        if notifications:
            print "Using local computer time"

    ntp_unaware = datetime.datetime.utcfromtimestamp(utc_unix_time)
    ntp_aware = local_tz.localize(ntp_unaware, is_dst=False)
    local_unix_time = (ntp_aware - local_tz.localize(datetime.datetime(1970, 1, 1), is_dst=False)).total_seconds()
    if notifications:
        print "Mayfly is being set to time zone %s" % local_tz
    return int(local_unix_time), ntp_aware


def parse_mayfly_set_response():
    # Parses the mayfly's response to the time-setting commands
    mayfly.readline()  # This line should be another rep of the Mayfly datetime
    mayfly.readline()  # This line should be "Recieved:###"
    diffts_abs = int(mayfly.readline().split()[4])
    setline = mayfly.readline()
    oldts = int(setline.split()[7])
    newts = int(setline.split()[4])
    return oldts, newts, diffts_abs


# Check all available serial ports
ports = list(serial.tools.list_ports.comports())

# Keep only ports with the Mayfly serial number (AH03IQ5AA)
mayfly_ports = [
    p for p in ports
    if 'AH03IQ5AA' in p[2]
]

# Give warnings if 0 or >1 Mayflies found
if not mayfly_ports:
    raise IOError("No Mayfly found")
if len(mayfly_ports) > 1:
    warnings.warn('Multiple Mayflies found - using the first')

# Open up the Mayfly serial port
mayfly = serial.Serial(str(mayfly_ports[0][0]), 9600, timeout=5)


# Wait for the Mayfly to initialize
print "Waiting for Mayfly to initialize"
time.sleep(2)
print mayfly.readline()

# Send the time to the Mayfly
print "First attempt to set the clock"
mayfly.write("T"+str(get_pc_time(notifications=True)[0]))
mf_resp1 = parse_mayfly_set_response()
print "Clock set to %s" % mf_resp1[1]

# Send the time to the Mayfly again to double-check the offsets
print "Checking the Mayfly response offset"
mayfly.readline()
mayfly.write("T"+str(get_pc_time()[0]))
mf_resp2 = parse_mayfly_set_response()


# Send the time to the Mayfly again adjusting for the offset
if mf_resp2[2] > 1:
    print "Mayfly takes %s seconds to respond to the PC time set command" % mf_resp2[2]
    print "Re-Adjusting the Mayfly Clock"
    mayfly.readline()
    mayfly.write("T"+str(get_pc_time()[0] + mf_resp2[2]))

print "Mayfly RTC is now within 1 second of computer or NTP clock"
print mayfly.readline()

mayfly.close()
