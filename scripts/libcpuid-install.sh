#!/bin/bash
#######################################################################
#
#  Script.......: libcpuid-install.sh
#  Creator......: matteyeux
#  Description..: Script to install libcpuid for sysnet
#  Type.........: Public
#
######################################################################
# Language :
#               bash
# Version : 1.0

srcdir=`dirname $0`
test -z "$srcdir"


function build_libcpuid(){
	git clone https://github.com/matteyeux/libcpuid
	cd $srcdir/libcpuid
	./autogen.sh
	make 
	sudo make install
}

function check4brew(){
	if [[ ! $(which brew) ]]; then
		echo "Brew is not installed, please install brew"
		exit 1
	fi
}

function build4linux (){

	if [[ ! $(which libtoolize) ]]; then
		sudo apt-get install libtool
	fi

	if [[ ! $(which autoreconf) ]]; then
		sudo apt-get install autotools-dev
	fi
	build_libcpuid
	sudo ldconfig
}

function build4macos (){
	if [[ ! $(which libtoolize) ]]; then
		check4brew
		brew install libtool
	fi

	if [[ ! $(which autoreconf) ]]; then
		check4brew
		brew install autotools-dev
	fi
}

if [[ $(uname) == "Linux" ]]; then
	build4linux
elif [[ $(uname) == "Darwin" ]]; then
	echo "rien"	
fi
