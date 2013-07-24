objects = gnublin-gpio
VERSION = 0.1
Maintainer = Manuel Liebert <man.liebert@gmail.com>

path = ../../
include ../../API-config.mk

all: $(path)gnublin.o $(objects)

gnublin.o: $(path)gnublin.cpp $(path)gnublin.h
	$(CXX) $(CXXFLAGS) $(BOARDDEF) -c $(path)gnublin.cpp 

$(objects): $(path)gnublin.o $(objects).cpp
	$(CXX) $(CXXFLAGS) $(BOARDDEF) -o $(objects) $(path)gnublin.o $(objects).cpp -I ../../

install: $(objects)
	cp $(objects) /usr/local/bin/

uninstall:
	rm -f /usr/local/bin/$(objects)
	
release: $(objects)
	@# generate .deb
	@mkdir -p deb/usr/bin
	@mkdir -p deb/DEBIAN

	@cp ./$(objects) deb/usr/bin/

	@# determine installed size of package
	@DEBSIZE=`du -c -k -s deb/usr/bin/$(objects) | tail -n 1 | gawk '/[0-9]/ { print $1 }'`

	@# create control file
	@echo "Package: $(objects)" > deb/DEBIAN/control
	@echo "Version: $(VERSION)" >> deb/DEBIAN/control
	@echo "Section: devel" >> deb/DEBIAN/control
	@echo "Priority: optional" >> deb/DEBIAN/control
	@echo "Architecture: $(Architecture)" >> deb/DEBIAN/control
	@echo "Essential: no" >> deb/DEBIAN/control
	@echo "Depends: " >> deb/DEBIAN/control
	@echo "Installed-Size: $(DEBSIZE)"  >> deb/DEBIAN/control
	@echo "Maintainer: $(Maintainer)" >> deb/DEBIAN/control
	@echo "Description: Gnublin tool" >> deb/DEBIAN/control

	@chmod +x deb/usr/bin/$(objects)

	@# build package
	@dpkg -b deb/ $(objects).deb

clean: 
	rm -Rf *.o $(objects) deb/ $(objects).deb
