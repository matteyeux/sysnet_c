#!/bin/bash
git clone https://github.com/matteyeux/libcpuid
cd libcpuid
./autogen.sh
make

if [[ $1 == "drone" ]]; then
	make install
	ldconfig
else
	sudo make install
	sudo ldconfig
fi

cd ..
make
./sysnet -a && ./sysnet -d /
./sysnet -n list
./sysnet -n eth0
./sysnet -v
