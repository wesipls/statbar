#!/bin/bash

# Usage: ./disk_temp.sh
# Example output:  +35°C
#
# Fetch and display the temperature of $disk
#
# Assumes disk is /dev/sdb, modify as needed.
#
# Should work on most Linux systems with smartmontools installed.

disk=sdb
sudo smartctl -a /dev/$disk | grep Temperature | awk -F " " '{print " +"$10 "°C"}'
