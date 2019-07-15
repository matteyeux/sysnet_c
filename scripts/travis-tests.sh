#!/bin/bash
git clone https://github.com/matteyeux/libcpuid
cd libcpuid
./autogen.sh
make

if [[ $1 == "drone" ]]; then
	make install
	ldconfig
	apt update
	apt install libiw-dev -y
else
	sudo make install
	sudo ldconfig
	sudo apt update
	sudo apt install libiw-dev -y
fi

cd ..
make
./sysnet -a && ./sysnet -d /
./sysnet -n list
./sysnet -n eth0
./sysnet -v
