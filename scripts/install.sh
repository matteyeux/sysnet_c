#!/bin/bash
#######################################################################
#
#  Script.......: install.sh
#  Creator......: matteyeux
#  Description..: Script to install sysnet
#  Type.........: Public
#
######################################################################
# Language :
#               bash
# Version : 1.0


function depends(){
	echo -e "\033[1;32m[i] installing dependencies...\033[0m"
	if [[ $(uname) == "Darwin" ]]; then
		check4brew
		brew install libtool autotools-dev
	elif [[ $(uname) == "Linux" ]]; then
		sudo apt-get install -qq -y m4 autoconf libtool autotools-dev build-essential make libiw-dev
	else
		echo "Sorry, your operating system is not supported by this script yet :-("
	fi
}

function install_libcpuid(){
	if [[ $(arch) == "x86_64" || $(arch) == "i686" || $(arch) == "i386" ]]; then
		git clone https://github.com/anrieff/libcpuid.git
		cd libcpuid
		#cp /usr/share/libtool/build-aux/ltmain.sh .
		libtoolize
		autoreconf --install
		./configure
		make && sudo make install && cd ..
	else 
		echo "skipping libcpuid"
	fi
}

function check4brew(){
	if [[ ! $(which brew) ]]; then
		echo -e "\033[31m[e] Brew is not installed, please install brew\033[0m"
		echo -e "\033[31mhttps://brew.sh\033[0m"
		exit 1
	fi
}

depends
install_libcpuid
if [[ -d .git ]]; then
	sudo make install		# I assume you are in sysnet dir
else
	sudo make -C $(dirname $(pwd)) install
fi

sudo ldconfig
