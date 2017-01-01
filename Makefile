CC = gcc
uname_s = $(shell uname -s)
TARGET = sysnet
INSTALL_DIR ?= /usr/local/bin/
SRC = src
VERSION = 1.1

# Set cross toolchain, eg : CROSS_COMPILE=arm-linux-gnueabihf-
CROSS_COMPILE ?=

OBJECTS = src/main.o \
		  src/system.o \
		  src/network.o

all : $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)
	@echo "Successfully built $(TARGET) for $(uname_s)" 


$(SRC)/%.o : $(SRC)/%.c
	$(CC) -c $< -o $@

clean : 
	@echo "Cleaning..."
	@rm -rf src/*.o deb pideb \
	$(TARGET) \
	$(TARGET)_$(VERSION).deb 
	
install : $(TARGET)
	cp $(TARGET) $(INSTALL_DIR)

package: $(TARGET) $(RPI)
	@echo "Packing..."
	mkdir -p deb/usr/local/bin/ deb/DEBIAN
	cp $(TARGET) deb/usr/local/bin
	cp resources/control deb/DEBIAN
	dpkg-deb --build deb $(TARGET)_$(VERSION).deb
	@echo "done"
