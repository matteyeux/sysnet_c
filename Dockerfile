# Build Docker container for nmon's local test
# docker build -t matteyeux/sysnet_test .
# docker run -it matteyeux/sysnet_test bash
FROM ubuntu:18.04

MAINTAINER matteyeux <mathieu.hautebas@gmail.com>
# update
RUN apt-get update && echo "hi"

# install all depends
RUN apt-get -y install libtool autoconf build-essential git tar autotools-dev libiw-dev

# copy nmon. tip : it automatically unarchive nmon

RUN git clone https://github.com/anrieff/libcpuid.git 

RUN cd libcpuid && libtoolize && autoreconf --install && ./configure && make install && ldconfig

# copy sysnet. tip : it automatically unarchive sysnet
ADD sysnet.*.tar.gz sysnet
RUN make -C sysnet/sysnet DEBUG=1 && make -C sysnet/sysnet install

RUN echo "=== TESTS === " 
RUN sysnet -n && sysnet -s && sysnet -c && sysnet -d && sysnet -a
RUN sysnet -n list
RUN sysnet -n eth0
RUN sysnet -v 
