CC = gcc
uname_s = $(shell uname -s)
arch = $(shell arch)
TARGET = sysnet
INSTALL_DIR ?= /usr/local/bin/
SRC = src
VERSION = $(shell cat resources/control| grep Version | cut -d:  -f 2)
DEBUG ?=
DEBUGYESNO ?=
DBG ?=

ifeq ($(shell arch),x86_64) 
	LDFLAGS = -lcpuid
endif
ifeq ($(shell arch),i686)
	LDFLAGS = -lcpuid
endif
ifeq ($(shell arch),i386)
	LDFLAGS = -lcpuid
endif

ifeq ($(DEBUG), 1)
	DBG = -DDEBUG
	DEBUGYESNO = "with DEBUG flags"
	DEBUG-V = "-DEBUG"
endif

define print_success
	@echo "\033[0;32m$1\033[0m"
endef

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
		  src/cpu.o \
		  src/common.o

.PHONY : all test clean clean_all install tarball package
all : $(TARGET)

$(TARGET) : $(OBJECTS)
	@echo " LD	$(TARGET)"
	@$(CROSS_COMPILE)$(CC) $(OBJECTS) $(LDFLAGS) -o $(TARGET)
	$(call print_success, "Successfully built $(TARGET) version $(VERSION)$(DEBUG-V) on $(uname_s) for $(arch) $(DEBUGYESNO)")

$(SRC)/%.o : $(SRC)/%.c
	@echo " CC	$<"
	@$(CROSS_COMPILE)$(CC) -c $(DBG) -I. $< -o $@

test : tarball
	docker build -t matteyeux/sysnet_test .

clean : 
	rm -rf src/*.o deb $(TARGET)*

clean_all : clean
	rm -rf deb *.deb $(TARGET)_$(VERSION)_$(arch).deb \
		   release

install : $(TARGET)
	install -v $(TARGET) $(INSTALL_DIR)

tarball : clean
	tar zcvf ../$(TARGET).$(VERSION).tar.gz ../$(TARGET)
	mv ../$(TARGET).$(VERSION).tar.gz .

# make CROSS_COMPILE=arm-linux-gnueabihf- package
package: $(TARGET)
	@echo "Packing..."
	mkdir -p release
	mkdir -p deb/usr/local/bin/ deb/DEBIAN
	cp $(TARGET) deb/usr/local/bin
	cp resources/control deb/DEBIAN
	dpkg-deb --build deb release/$(TARGET)_$(VERSION)_$(arch).deb
	@echo "done"
