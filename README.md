# SYSNET

[![Build Status](https://travis-ci.org/matteyeux/sysnet.svg?branch=master)](https://travis-ci.org/matteyeux/sysnet)
[![Platform](https://img.shields.io/badge/platform-multiples-yellowgreen.svg)](https://github.com/matteyeux/sysnet#platforms-tested-) 
[![Packagist](https://img.shields.io/badge/license-MIT-orange.svg)](https://github.com/matteyeux/sysnet/blob/master/LICENSE)&nbsp;
[![Packagist](https://img.shields.io/badge/contact-matteyeux-blue.svg)](https://twitter.com/matteyeux) 

```
Usage : sysnet [OPTIONS]
 -s, --system   print system informations
 -n, --network  print network informations
 -a, --all      print all informations
 -v, --version  print version
```

Sysnet is a tool which recovers system & network information.
This tool supports Linux, OS X Windows & [iOS](https://github.com/theos/theos)

###  System 

```
User : 			mathieu
Operating System :	Linux
version :		4.6.4-301.fc24.x86_64
architecture : 		x86_64
n° of processes : 	395
shell : 		/bin/bash
Disk usage of / : 	5501992960 
Free space in / : 	23432278016
Total : 		28934270976

```

### Network

```
hostname: localhost.localdomain
lo
	address: 127.0.0.1
	netmask: 255.0.0.0	suffix : 8
ens33
	address: 192.168.72.133
	netmask: 255.255.255.0	suffix : 24
IPv6 lo
	address: ::1
IPv6 ens33
	address: fe80::bb0f:b597:52ce:ba08%ens33

Gateway : 192.168.72.2
```



### Demo

Here is a demo of sysnet running on GNU/Linux Debian 8.5. 

[![sysnet demo](https://asciinema.org/a/6jo8dd7d66ljrso5xon8ob5ub.png)](https://asciinema.org/a/6jo8dd7d66ljrso5xon8ob5ub)

### Platforms tested :

- Linux
- macOS
- Windows
- iOS 9

### TODO

The TODO list is [here](https://github.com/matteyeux/sysnet/projects/1)

### Installation 

I wrote a Makefile to build sysnet run `make` & `sudo make install` to build and install

Make sure you have GCC installed

By default, the install directory is `/usr/bin/`, you can change it by modifying `INSTALL_DIR` variable in the [Makefile](https://github.com/matteyeux/sysnet/blob/master/Makefile#L4) 

You can now cross-compile sysnet for Raspberry Pi and also build and `.deb` to install it. 
Make sure to have the toolchain setup in your $PATH.

For Ubuntu : `sudo apt-get install gcc-arm-linux-gnueabihf`

Then run `make rpi` to build sysnet for Raspberry Pi <br>

### Credits

Tool developed by [@matteyeux](https://twitter.com/matteyeux)
