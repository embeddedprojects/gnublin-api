
all: build gnublin.o gnublin.a libgnublin.so.1.0.1

build:
	sh build-API.sh

gnublin.o: gnublin.cpp gnublin.h
	arm-linux-gnueabi-g++ -Wall -c gnublin.cpp 

gnublin.a: gnublin.o 
	ar rcs gnublin.a gnublin.o

libgnublin.so.1.0.1: gnublin.cpp gnublin.h
	arm-linux-gnueabi-g++ -Wall -c -fPIC gnublin.cpp -o gnublin_fpic.o     
	arm-linux-gnueabi-g++ -shared -Wl,-soname,libgnublin.so.1 -o libgnublin.so.1.0.1  gnublin_fpic.o

clean: 
	rm -Rf *.o gnublin.a libgnublin.so.1.0.1
