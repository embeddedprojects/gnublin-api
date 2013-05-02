objects = gnublin-adcmod
path = ../../

all: $(path)gnublin.o $(objects)

$(path)gnublin.o: $(path)gnublin.cpp $(path)gnublin.h
	$(MAKE) -C $(path) gnublin.o

$(objects): $(path)gnublin.o $(objects).cpp
	arm-linux-gnueabi-g++ -o $(objects) $(path)gnublin.o $(objects).cpp -I ../../

install: $(objects)
	cp $(objects) /usr/local/bin/

uninstall:
	rm -f /usr/local/bin/$(objects)

clean: 
	rm -Rf *.o $(objects)
