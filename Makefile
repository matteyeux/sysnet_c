CC = gcc
uname_s = $(shell uname -s)
arch = $(shell arch)
TARGET = sysnet
INSTALL_DIR ?= /usr/local/bin/
SRC = src
VERSION = 1.1.2

ifeq ($(shell arch),x86_64) 
	LDFLAGS = -lcpuid
endif
ifeq ($(shell arch),i686)
        LDFLAGS = -lcpuid
endif
ifeq ($(shell arch),i386)
        LDFLAGS = -lcpuid
endif

# Set cross toolchain, eg : CROSS_COMPILE=arm-linux-gnueabihf-
CROSS_COMPILE ?=
ifeq ($(CROSS_COMPILE),arm-linux-gnueabihf-) 
	LDFLAGS =
	arch = $(shell echo "$(CROSS_COMPILE)" | cut -f 1 -d -)
endif
ifeq ($(CROSS_COMPILE),aarch64-linux-gnu-) 
	LDFLAGS =
	arch = $(shell echo "$(CROSS_COMPILE)" | cut -f 1 -d -)
endif

OBJECTS = src/main.o \
		  src/system.o \
		  src/network.o \
		  src/cpu.o

all : $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CROSS_COMPILE)$(CC) $(OBJECTS) $(LDFLAGS) -o $(TARGET)
	@echo "Successfully built $(TARGET) for $(uname_s) on $(arch)" 


$(SRC)/%.o : $(SRC)/%.c
	$(CROSS_COMPILE)$(CC) -c -I. $< -o $@

clean : 
	rm -rf src/*.o deb $(TARGET)*

clean_all : clean
	rm -rf deb *.deb $(TARGET)_$(VERSION)_$(arch).deb 

install : $(TARGET)
	cp $(TARGET) $(INSTALL_DIR)

# make CROSS_COMPILE=arm-linux-gnueabihf- package
package: $(TARGET)
	@echo "Packing..."
	mkdir -p deb/usr/local/bin/ deb/DEBIAN
	cp $(TARGET) deb/usr/local/bin
	cp resources/control deb/DEBIAN
	dpkg-deb --build deb $(TARGET)_$(VERSION)_$(arch).deb
	@echo "done"
