#!/bin/bash

#Usage: ./weather.sh
#Example output: ☀️   0mm   ↘5km/h   +15°C   (+13°C)
#
#Fetches weather data from wttr.in API.
#Assumes location is Espoo, change accordingly in the curl URL if needed.
#
#Stores weather data in /tmp/weather and refreshes it if older than 30 minutes.

weather=$(cat /tmp/weather)
echo $weather | awk -F " " '{print $1 "   " $2 "   " $3 "   " $4 "   (" $5")"}'

if [ -e /tmp/weather ]; then
  if [ ! -s /tmp/weather ] || [ -z "$(cat /tmp/weather)" ] || [ $(($(date +%s) - $(stat -c %Y /tmp/weather))) -gt 1800 ]; then
    weather=$(curl -s https://wttr.in/Espoo?format="%c+%p+%w+%t+%f")
    echo "$weather" >/tmp/weather
  fi
else
  weather=$(curl -s https://wttr.in/Espoo?format="%c+%p+%w+%t+%f")
  echo "$weather" >/tmp/weather
fi
