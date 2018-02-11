#!/bin/bash

WD=~/.tmp_cross_install #Working Directory

function check_dir(){
	if [[ ! -d  "/usr/aarch64-linux-gnu" ]]; then
		sudo apt-get update; sudo apt-get install gcc-aarch64-linux-gnu  -qq -y
	fi

	if [[ ! -d "/usr/arm-linux-gnueabihf" ]]; then
		sudo apt-get update; sudo apt-get install gcc-arm-linux-gnueabihf -qq -y
	fi

	if [[ ! -d "/usr/arm-linux-gnueabihf" && ! -d  "aarch64-linux-gnu" ]]; then
		sudo apt-get update
		sudo apt-get install gcc-arm-linux-gnueabihf -qq -y
		sudo apt-get install gcc-aarch64-linux-gnu gcc-arm-linux-gnueabihf -qq -y
	fi
}

function armhf_setup(){
	# download armhf deb for raspberry pi
	wget http://ftp.us.debian.org/debian/pool/main/w/wireless-tools/libiw-dev_30~pre9-12+b1_armhf.deb

	# extract stuff from deb
	dpkg -x libiw-dev_30\~pre9-12+b1_armhf.deb libiw-armhf

	# install cross compiler stuff to cross compiler directory for arm-gnu
	sudo cp libiw-armhf/usr/include/*.h /usr/arm-linux-gnueabihf/include
	sudo cp libiw-armhf/usr/lib/arm-linux-gnueabihf/libiw.a /usr/arm-linux-gnueabihf/lib/
	sudo ln -s /usr/arm-linux-gnueabihf/lib/libiw.a /usr/arm-linux-gnueabihf/lib/libiw.so
}

function aarch64_setup(){
	wget http://ftp.us.debian.org/debian/pool/main/w/wireless-tools/libiw-dev_30~pre9-12+b1_arm64.deb
	dpkg -x libiw-dev_30\~pre9-12+b1_arm64.deb libiw-aarch64

	sudo cp libiw-aarch64/usr/include/*.h /usr/aarch64-linux-gnu/include
	sudo cp libiw-aarch64/usr/lib/aarch64-linux-gnu/libiw.a /usr/aarch64-linux-gnu/lib/
	sudo ln -s /usr/aarch64-linux-gnu/lib/libiw.a /usr/aarch64-linux-gnu/lib/libiw.so
}

if [[ -e $WD ]];then 
	rm -rf $WD
fi

mkdir $WD
cd $WD
check_dir
armhf_setup
aarch64_setup
