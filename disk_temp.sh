#!/bin/bash

# Usage: ./disk_temp.sh
# Example output:  +35°C
#
# Fetch and display the temperature of $disk
#
# Change $disk to the uuid of the disk you want to get temperature for.
#
# Should work on most Linux systems with smartmontools installed.
# Needs sudo permissions for smartmontools to access disk information, to add it for just smartmontools add this to the /etc/sudoers file: ALL=(ALL) NOPASSWD: /usr/sbin/smartctl

disk=8b33bb8a-2621-4d06-9645-9809f09e660b
sudo smartctl -a /dev/disk/by-uuid/$disk | grep Temperature | awk -F " " '{print " +"$10 "°C"}'
