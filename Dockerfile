# Build Docker container for nmon's local test
# docker build -t matteyeux/nmon_test .
FROM ubuntu:16.04

MAINTAINER matteyeux <mathieu.hautebas@gmail.com>
# update
RUN apt-get update && echo "hi"

# install all depends
RUN apt-get -y install libtool autoconf build-essential git tar autotools-dev

# copy nmon. tip : it automatically unarchive nmon
ADD sysnet.1.1.2.tar.gz sysnet

RUN git clone https://github.com/anrieff/libcpuid.git 

RUN cd libcpuid && libtoolize && autoreconf --install && ./configure && make install && ldconfig

RUN make -C sysnet/sysnet DEBUG=1 && make -C sysnet/sysnet install

RUN echo "=== TESTS === " 
RUN sysnet -n && sysnet -s && sysnet -c && sysnet -d && sysnet -a 
RUN sysnet -v 
