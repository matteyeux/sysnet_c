CC = gcc
uname_s = $(shell uname -s)
arch = $(shell arch)
TARGET = sysnet
LIB = libsysnet.a
INSTALL_DIR ?= /usr/local/bin/
SRC = src
VERSION = $(shell cat resources/control| grep Version | cut -d:  -f 2)
DEBUG ?=
DEBUGYESNO ?=
DBG ?=
CFLAGS := -I. -Wall -Wextra
LDFLAGS := -lm -liw

ifeq ($(shell arch),x86_64) 
	LIBCPUID = -DLIBCPUID
	LDFLAGS += -lcpuid
endif
ifeq ($(shell arch),i686)
	LIBCPUID = -DLIBCPUID
	LDFLAGS += -lcpuid
endif
ifeq ($(shell arch),i386)
	LIBPUID = -DLIBCPUID
	LDFLAGS += -lcpuid
endif

ifeq ($(DEBUG), 1)
	DBG = -DDEBUG
	DEBUGYESNO = "with DEBUG flags"
	DEBUG-V = "-DEBUG"
	CFLAGS += -g
endif

define print_success
	@echo "\033[0;32m$1\033[0m"
endef

# Set cross toolchain, eg : CROSS_COMPILE=arm-linux-gnueabihf-
ifeq ($(CC),arm-linux-gnueabihf-gcc)
	LIBCPUID =
	LDFLAGS = -lm -liw
	arch = $(shell echo "$(CC)" | cut -f 1 -d -)
endif

ifeq ($(CC),aarch64-linux-gnu-gcc)
	LIBCPUID =
	LDFLAGS = -lm -liw
	arch = $(shell echo "$(CC)" | cut -f 1 -d -)
endif

SOURCES = $(wildcard src/*.c)

OBJECTS = $(SOURCES:.c=.o)

.PHONY : all test clean clean_all install tarball package

all : $(TARGET)

$(TARGET) : $(OBJECTS)
	@echo " LD	$(TARGET)"
	@$(CC) $(OBJECTS) $(LDFLAGS) -o $(TARGET)
	$(call print_success, "Successfully built $(TARGET) version $(VERSION)$(DEBUG-V) on $(uname_s) for $(arch) $(DEBUGYESNO)")

$(LIB) : $(OBJECTS)
	@echo " AR	$(LIB)"
	@rm src/main.o
	@ar rc $(LIB) $(OBJ)
	@ranlib $(LIB)

$(SRC)/%.o : $(SRC)/%.c
	@echo " CC	$<"
	@$(CC) $(CFLAGS) -c $(DBG) $(LIBCPUID) $< -o $@

test : tarball
	docker build -t matteyeux/sysnet_test .

clean : 
	rm -rf src/*.o deb $(LIB) $(TARGET)*

clean_all : clean
	rm -rf deb *.deb $(TARGET)_$(VERSION)_$(arch).deb release

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
