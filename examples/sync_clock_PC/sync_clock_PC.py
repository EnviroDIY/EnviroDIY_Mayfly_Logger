# -*- coding: utf-8 -*-

"""
Created by Sara Geleskie Damiano on 1/9/2017 at 2:24 PM

__author__ = 'Sara Geleskie Damiano'
__contact__ = 'sdamiano@stroudcenter.org'

This interacts with an FTDI connected Arduino device (like the EnviroDIY Mayfly)
running sync_clock_PC.ino to synchronize the RTC chip to Network Time Protocol clocks
(or to local PC time is NTP is unavailable)

"""


import datetime
import time
import warnings

import ntplib
import serial.tools.list_ports


# from tzlocal import get_localzone


# get local timezone
# local_tz = get_localzone()


def get_device_time():
    # A helper function to get the current time from a device running sync_clock_PC.ino
    # Returns the current time as a time-zone aware python date-time object
    device_time_str = device.readline()
    # print device_time_str
    device_rtc = int(device_time_str.split("(")[1].split(")")[0])
    # Convert into a time-zone aware python datetime value
    # device_rtc_unaware = datetime.datetime.utcfromtimestamp(device_rtc)
    # device_rtc_aware = local_tz.localize(device_rtc_unaware, is_dst=False)
    return device_rtc  # , device_rtc_aware


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

    # Conversions to deal with local vs. UTC time
    # ntp_unaware = datetime.datetime.utcfromtimestamp(utc_unix_time)
    # ntp_aware = local_tz.localize(ntp_unaware, is_dst=False)
    # local_unix_time = (ntp_aware - datetime.datetime(1970, 1, 1)).total_seconds()
    if notifications:
        # print "RTC chip is being set to time zone %s" % local_tz
        print "RTC chip is being set to UTC"
        print "Please account for timezones in your sketch"
    return int(utc_unix_time)  # , ntp_aware


def parse_device_set_response():
    # Parses the device's response to the time-setting commands
    device.readline()  # This line should be another rep of the device datetime
    device.readline()  # This line should be "Recieved:###"
    diffts_abs = int(device.readline().split()[4])
    setline = device.readline()
    oldts = int(setline.split()[7])
    newts = int(setline.split()[4])
    return oldts, newts, diffts_abs


# Check all available serial ports
ports = list(serial.tools.list_ports.comports())

# Keep only ports with the device communication type
device_ports = [
    p for p in ports
    if 'VID_0403+PID_6001' in p[2]
    ]

# Give warnings if 0 or >1 Mayflies found
if not device_ports:
    print "No FTDI device found"
    raw_input("Press Enter to Exit")
    exit()
elif len(device_ports) > 1:
    warnings.warn('Multiple FTDI devices found - using %s' % device_ports[0][1])
else:
    print "FTDI Device found at %s" % device_ports[0][1]

# Open up the device serial port
try:
    device = serial.Serial(str(device_ports[0][0]), 57600, timeout=5)
except:
    print "Cannot access %s" % device_ports[0][1]
    print "Please close any other programs accessing the serial port"
    raw_input("Press Enter to Exit")
    exit()

# Wait for the device to initialize
print "Waiting for device to initialize"
time.sleep(2)
timeout_time = time.time() + 10
print device.readline()

# Check that getting expected responses from the device
try:
    get_device_time()
except:
    print "Device is not sending expected output."
    print "Please ensure that sync_clock_PC.ino has been uploaded to the device"
    raw_input("Press Enter to Exit")
    exit()

# Send the time to the device
print "First attempt to set the clock"
device.write("T" + str(get_pc_time(notifications=True)))
mf_resp1 = parse_device_set_response()
print "Clock set to %s" % mf_resp1[1]

# Send the time to the device again to double-check the offsets
print "Checking the device response offset"
device.readline()
device.write("T" + str(get_pc_time()))
mf_resp2 = parse_device_set_response()

# Send the time to the device again adjusting for the offset
if mf_resp2[2] > 1:
    print "Device takes %s seconds to respond to the PC time set command" % mf_resp2[2]
    print "Re-adjusting the device clock"
    device.readline()
    device.write("T" + str(get_pc_time() + mf_resp2[2]))

print "Device RTC is now within 1 second of computer or NTP clock"
print device.readline()

device.close()

raw_input("Press Enter to Exit")
exit()
