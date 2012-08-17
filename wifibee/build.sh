#!/bin/bash
echo clean build upload and serial monitor
echo To exit from serial monitor press CTRL A then CTRL X
ino clean; ino build -m atmega328 && ino upload -m atmega328 && ino serial -b 57600
