#!/bin/bash
# Keep the ttyUSB0 device open on fd 3
exec 3<>/dev/ttyUSB0
stty -F /dev/ttyUSB0 9600 cs8 -cstopb -parenb
echo "AT" > &3                 # send data
cat < &3                      # read the data

