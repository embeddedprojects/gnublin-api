objects = clock
 
all: gnublin.o $(objects)
 
gnublin.o: gnublin.cpp gnublin.h
	arm-linux-gnueabi-g++ -c gnublin.cpp 
 
$(objects): $(objects).cpp gnublin.o
	arm-linux-gnueabi-g++ -o $(objects) $(objects).cpp gnublin.o -Wall
 
clean: 
	rm -Rf *.o $(objects)

