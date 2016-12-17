CC = gcc
PREFIX = arm-linux-gnueabihf-

uname_s = $(shell uname -s)

TARGET = sysnet
RPI = rpi

INSTALL_DIR = /usr/local/bin/
SRC = src
RPI_SRC = SRC

VERSION = 1.1


OBJECTS = src/main.o \
		  src/system.o \
		  src/network.o

RPI_OBJECTS = src/main.arm.o \
		      src/system.arm.o \
		      src/network.arm.o

all : $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)
	@echo "Successfully built $(TARGET) for $(uname_s)" 

$(RPI) : $(RPI_OBJECTS)
	$(PREFIX)$(CC) $(RPI_OBJECTS) -o $(TARGET).$(RPI)
	@echo "Successfully built $(TARGET) for Raspberry Pi"

$(SRC)/%.o : $(SRC)/%.c
	$(CC) -c $< -o $@

$(SRC)/%.arm.o : $(SRC)/%.c
	$(PREFIX)$(CC) -c $< -o $@

clean : 
	@echo "Cleaning..."
	@rm -rf src/*.o deb pideb \
	$(TARGET) $(TARGET).$(RPI) \
	$(TARGET)_$(VERSION)_$(shell arch).deb \
	$(TARGET)_$(RPI)_$(VERSION).deb
	
install : $(TARGET)
	cp $(TARGET) $(INSTALL_DIR)

package: $(TARGET) $(RPI)
	@echo "Packing..."
	mkdir -p pideb/usr/local/bin pideb/DEBIAN
	mkdir -p deb/usr/local/bin/ deb/DEBIAN
	cp $(TARGET).$(RPI) pideb/usr/local/bin/
	cp $(TARGET) deb/usr/local/bin
	cp resources/control pideb/DEBIAN
	cp resources/control deb/DEBIAN
	dpkg-deb --build deb $(TARGET)_$(RPI)_$(VERSION).deb
	dpkg-deb --build deb $(TARGET)_$(VERSION)_$(shell arch).deb
	@echo "done"



