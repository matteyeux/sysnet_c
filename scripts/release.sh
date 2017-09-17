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
# Version : 0.2

version=`cat resources/control | grep Version | cut -d:  -f 2`
hashfile="release/hash_$version.txt"
compiler=( "" "arm-linux-gnueabihf-" "aarch64-linux-gnu-")


for i in "${compiler[@]}"
do
	make clean
	make CROSS_COMPILE=$i package
done

if [[ -e $hashfile ]]; then
	rm -rf $hashfile
fi
shasum -a 512 release/* >>  $hashfile
