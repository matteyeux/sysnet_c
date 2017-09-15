#!/bin/bash
#######################################################################
#
#  Script.......: release.sh
#  Creator......: matteyeux
#  Description..: Script to to build release debs for x86_64, ARM and aarch64 
#                 Run in same dir as Makefile ./scripts/release.sh
#  Type.........: Public
#
######################################################################
# Language :
#               bash
# Version : 1.0
cc=( "" "arm-linux-gnueabihf-" "aarch64-linux-gnu-")

for i in "${cc[@]}"
do
	make clean
	make CROSS_COMPILE=$i package
done
