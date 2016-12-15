CC = gcc
CCRPI = arm-linux-gnueabihf-gcc
uname_s = $(shell uname -s)
TARGET = sysnet
INSTALL_DIR = /usr/local/bin/
SRC = src
VERSION = 1.0


OBJECTS = src/main.o \
		  src/system.o \
		  src/network.o

all : $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) -o $(TARGET) src/main.o src/system.o src/network.o
	@echo "Successfully built $(TARGET) for $(uname_s)" 

$(SRC)/%.o : $(SRC)/%.c
	$(CC) -c $< -o $@

clean : 
	rm -rf src/*.o deb $(TARGET) $(TARGET)_$(VERSION).deb
	
install : $(TARGET)
	cp $(TARGET) $(INSTALL_DIR)

package: $(TARGET)
	@echo "Packing..."
	mkdir -p deb/usr/local/bin/ deb/DEBIAN
	cp $(TARGET) deb/usr/local/bin
	cp resources/control deb/DEBIAN
	dpkg-deb --build deb $(TARGET)_$(VERSION).deb
	@echo "done"

