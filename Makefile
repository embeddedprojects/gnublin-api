OBJ := dogm lcd lm75 pca9555 relay step adcmod
SUBDIRS := $(OBJ:%=gnublin-tools/gnublin-%)
CLEANDIRS := $(SUBDIRS:%=clean-%)
INSTALLDIRS := $(SUBDIRS:%=install-%)
UNINSTALLDIRS := $(SUBDIRS:%=uninstall-%)
include variables.mk

.PHONY: gnublin-tools $(SUBDIRS) $(CLEANDIRS) $(INSTALLDIRS)
     
all: build gnublin.o gnublin.a libgnublin.so.1.0.1 gnublin-tools

build:
	sh build-API.sh

gnublin.o: build gnublin.cpp gnublin.h
	$(CXX) $(CXXFLAGS) $(BOARDDEF) $(APIDEFS) -c gnublin.cpp

gnublin.a: gnublin.o 
	ar rcs gnublin.a gnublin.o

libgnublin.so.1.0.1: gnublin.cpp gnublin.h
	$(CXX) $(CXXFLAGS) -c -fPIC gnublin.cpp -o gnublin_fpic.o     
	$(CXX) -shared -Wl,-soname,libgnublin.so.1 -o libgnublin.so.1.0.1  gnublin_fpic.o

#build gnublin-tools
gnublin-tools: gnublin.o $(SUBDIRS) 
$(SUBDIRS):
	$(MAKE) -C $@

#Install
install: $(INSTALLDIRS)
$(INSTALLDIRS): 
	$(MAKE) -C $(@:install-%=%) install

#Uninstall
uninstall: $(UNINSTALLDIRS)
$(UNINSTALLDIRS):
	$(MAKE) -C $(@:uninstall-%=%) uninstall

#clean
clean: $(CLEANDIRS)
	rm -Rf *.o gnublin.a libgnublin.so.1.0.1
$(CLEANDIRS): 
	$(MAKE) -C $(@:clean-%=%) clean
