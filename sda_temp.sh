#!/bin/bash
sudo smartctl -a /dev/sda | grep Temperature | awk -F " " '{print " +"$10 "°C"}'
