import pyowm
from datetime import datetime
import tzlocal
import time
import requests
import json

# Set variables for the POST request
url = 'http://data.envirodiy.org/api/data-stream/'

# These UUIDs come from the Site details page in the web application
token = 'ac8502b6-2ba4-4b2c-a504-9df0af70360b'
samplingFeatureUUID = '1aa8f9f7-e5b5-4942-8b2d-a8b44fd93efb'
resultUUID = 'a76b27aa-489b-435a-bd49-d4c81968eaca'

# Create the header information for the POST request
headers = {'TOKEN': token,
           'Cache-Control': 'no-cache',
           'Content-Type': 'application/json'}

# Set up some program status variables
# currTime = 0
# prevTime = 0
# counter = 0

# Get the local time zone
localTimeZone = tzlocal.get_localzone()

# To test the POST requests, get data from the Open Weather Map API
# This data source was arbitrarily selected just so I could post some data without a datlogger or sensors
# Provide a valid API key for open weather map
# You can get your own API key by creating an account at http://openweathermap.org/
owm = pyowm.OWM('d08c27593a95bb47d71c657b32b7cd02')

# Main program loop - runs until you kill it
# ------------------------------------------
# while True:

# Get the current weather in Logan, UT from the Open Weather Map API
observation = owm.weather_at_place('Coatesville, PA')
w = observation.get_weather()

# The Open Weather Map only updates the observation every 30 minutes or so
# Get the time at which the observation was made and make sure
# we have a new data point before sending it to the web service
# If the observation is the same as the last iteration, wait 90 seconds and check again for a new observation
# Also - skip the very first observation the program gets because otherwise I'll end up adding the
# same point to the database over and over every time I restart the script in testing.
# if currTime != prevTime and counter > 1:  # This is a new observation
# Get the temperature out of the observation
currTemp = w.get_temperature('celsius').get("temp")

# Convert the current time from UTC to a LocalDateTime string
# currTime = w._reference_time
# localTime = datetime.fromtimestamp(currTime, localTimeZone)
# currTimeString = localTime.strftime('%Y-%m-%d %H:%M:%S')

currTimeString = datetime.now().strftime('%Y-%m-%d %H:%M:%S')

# Print the current time and temperature to the console so I get a warm fuzzy that its working
print(currTimeString)
print(currTemp)

# Construct the payload for the REST request to the data.envirodiy.org server
payLoad = {'sampling_feature': samplingFeatureUUID,
           'timestamp': currTimeString,
           resultUUID: str(currTemp)}

# Send the POST request to the server and print the status code of the response to the console
response = requests.post(url, data=json.dumps(payLoad), headers=headers)
print(response.status_code)

req = requests.Request('POST', url, headers=headers, data=json.dumps(payLoad))
prepared = req.prepare()


def pretty_print_POST(req):
    """
    At this point it is completely built and ready
    to be fired; it is "prepared".

    However pay attention at the formatting used in
    this function because it is programmed to be pretty
    printed and may differ from the actual request.
    """
    print('{}\n{}\n{}\n\n{}'.format(
        '-----------START-----------',
        req.method + ' ' + req.url,
        '\n'.join('{}: {}'.format(k, v) for k, v in req.headers.items()),
        req.body,
    ))

pretty_print_POST(prepared)

    # Set the value of the counter so I know not to skip any subsequent records.
    # counter += 1

    # Pause the program for 90 seconds before checking the temperature again
    # time.sleep(90)

