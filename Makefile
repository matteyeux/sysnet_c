CC = gcc
uname_s = $(shell uname -s)
TARGET = sysnet
INSTALL_DIR = /usr/local/bin/
VERSION = 1.0
all : $(TARGET)
$(TARGET) : src/main.o src/system.o src/network.o
	$(CC) -o $(TARGET) src/main.o src/system.o src/network.o
	@echo "Successfully built $(TARGET) for $(uname_s)" 

main.o : src/main.c
	$(CC) -c src/main.c -o src/main.o
	
system.o : src/system.c
	$(CC) -c src/system.c -o src/system.o

network.o : src/network.c
	$(CC) -c src/network.c -o src/network.o

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

