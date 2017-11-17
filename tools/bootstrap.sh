#!/bin/sh

cd /pax-driver
make
insmod ttyPos.ko
chmod 666 /dev/ttyPos0
