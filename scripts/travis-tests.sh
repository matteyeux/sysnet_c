#!/bin/bash
git clone https://github.com/matteyeux/libcpuid
cd libcpuid
./autogen.sh
make
sudo make install
if [[ $(uname) == 'Linux' ]];then
        sudo ldconfig
fi
cd ..
make
./sysnet -a && ./sysnet -d /
./sysnet -n list
./sysnet -n eth0
./sysnet -f .
./sysnet -v
