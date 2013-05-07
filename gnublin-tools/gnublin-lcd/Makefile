objects = gnublin-lcd
path = ../../
include ../../API-config.mk

all: gnublin.o $(objects)

gnublin.o: $(path)gnublin.cpp $(path)gnublin.h
	$(CXX) $(CXXFLAGS) $(BOARDDEF) -c $(path)gnublin.cpp 

$(objects): gnublin.o $(objects).cpp
	$(CXX) $(CXXFLAGS) $(BOARDDEF) -o $(objects) gnublin.o $(objects).cpp -I ../../

install: $(objects)
	cp $(objects) /usr/local/bin/

uninstall:
	rm -f /usr/local/bin/$(objects)

clean: 
	rm -Rf *.o $(objects)
