# SYSNET

[![Build Status](https://travis-ci.org/matteyeux/sysnet.svg?branch=master)](https://travis-ci.org/matteyeux/sysnet)
[![Platform](https://img.shields.io/badge/platform-multiples-yellowgreen.svg)](https://github.com/matteyeux/sysnet#platforms-tested-) 
[![Packagist](https://img.shields.io/badge/license-MIT-orange.svg)](https://github.com/matteyeux/sysnet/blob/master/LICENSE)&nbsp;
[![Packagist](https://img.shields.io/badge/contact-matteyeux-blue.svg)](https://twitter.com/matteyeux) 

```
Usage : sysnet [OPTIONS]
 -s, --system	system information
 -n, --network	network information
 -c, --cpu	cpu information
 -d, --disk	disk information
 -a, --all	all information
 -v, --version	version
```

Sysnet is a tool which grabs system & network information.
This tool supports Linux, OS X, & [iOS](https://github.com/theos/theos)

###  System 

```
User : 			mathieu
Operating System :	Linux
version :		3.16.0-4-amd64
architecture : 		x86_64
n° of processes : 	225
shell : 		/bin/bash

Used RAM : 		0.74 GB
Free RAM : 		3.12 GB
Total RAM : 	3.86 GB

```

### Network

```
hostname: pwn
lo
	address: 127.0.0.1
	netmask: 255.0.0.0		suffix : 8
eth0
	address: 192.168.181.128
	netmask: 255.255.255.0		suffix : 24
	broadcast: 192.168.181.255
IPv6 lo
	address: ::1
IPv6 eth0
	address: fe80::20c:29ff:feb0:ddf5%eth0

Gateway : 192.168.181.2
```

### Disk 
I also added support for disk information :

```
Disk usage of / : 	7.10 GB
Free space in / : 	30.50 GB
Total in /: 		37.60 GB
```

### CPU 
I am currently improving sysnet by supporting CPU information using [libcpuid](https://github.com/matteyeux/libcpuid)

```
Vendor :		GenuineIntel
Model :			Intel(R) Core(TM) i5-6200U CPU @ 2.30GHz
Physical cores :	2
```

### Demo

Here is a demo of sysnet running on GNU/Linux Debian 8.5. 

[![sysnet demo](https://asciinema.org/a/Ld5fe5Zy8qLBx9zcA8RKqQV3S.png)](https://asciinema.org/a/Ld5fe5Zy8qLBx9zcA8RKqQV3S)

### Platforms tested :

- Linux
- macOS
- iOS 6 - 10
- Windows Subsystem for Linux (WSL)

### TODO

The TODO list is [here](https://github.com/matteyeux/sysnet/projects/1)

### Installation 

Since v1.1 sysnet for Intel CPUs needs [libcpuid](https://github.com/matteyeux/libcpuid) to be built. I wrote a script to install `libcpuid`. <br>
Run `./scripts/libcpuid-install.sh`. <br>
This script works for Linux & macOS (you may have [brew](https://brew.sh) installed) <br>
Make sure you have GCC/clang installed

NO NEED FOR ARM ARCHITECTURE

Once libcpuid is installed run `make` to compile & `sudo make install` to install sysnet. <br>
By default, the install directory is `/usr/bin/`, you can change it by modifying `INSTALL_DIR` variable in the [Makefile](https://github.com/matteyeux/sysnet/blob/master/Makefile#L4) <br>
You can also cross-compile for any platform. You'd have to set `CROSS_COMPILE` with the correct toolchain. <br>
e.g : `make CROSS_COMPILE=arm-linux-gnueabihf-` <br>

You can also install pre-built packages for few Linux architectures :check [releases page](https://github.com/matteyeux/sysnet/releases)

### Credits

Tool developed by [@matteyeux](https://twitter.com/matteyeux)
