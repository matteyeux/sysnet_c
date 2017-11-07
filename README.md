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
With no argument here is the default output
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
If you want to list all interfaces which are up, run `sysnet -n list`
```bash
IPv4 lo
IPv4 enp1s0
IPv4 docker0
IPv6 lo
IPv6 enp1s0
IPv6 docker0
```
Then to print info about only one interface run `sysnet -n [interface]`. Eg : 
```
$ sysnet -n docker0
IPv4 docker0
        address: 172.17.0.1
        netmask: 255.255.0.0            suffix : 16
        broadcast: 172.17.255.255
IPv6 docker0
        address: fe80::42:aeff:fad6:9db%docker0
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

Since v1.1 sysnet for _some_ CPUs needs [libcpuid](https://github.com/anrieff/libcpuid) to be built. I wrote a script to install `libcpuid`. <br>
Run `./scripts/libcpuid-install.sh`. <br>
This script works for Linux & macOS (you may have [brew](https://brew.sh) installed) <br>
Make sure you have GCC/clang installed <br>

You will also have to install `libiw-dev` : `sudo apt-get install libiw-dev`
Once libcpuid and libiw-dev are installed run :
- `make` to build sysnet
- `sudo make install` to install sysnet. <br>

By default, the install directory is `/usr/local/bin/`. You can change it by modifying `INSTALL_DIR` variable in the [Makefile](https://github.com/matteyeux/sysnet/blob/master/Makefile#L4) <br>
You can also cross-compile for any platform. You'd have to set `CROSS_COMPILE` with the correct toolchain. <br>
e.g : `make CROSS_COMPILE=arm-linux-gnueabihf-` <br>

You can also install pre-built packages for few Linux architectures : check [releases page](https://github.com/matteyeux/sysnet/releases)

### Credits

Tool developed by [@matteyeux](https://twitter.com/matteyeux)
