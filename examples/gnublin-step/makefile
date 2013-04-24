CC          = arm-linux-gnueabi-g++
TARGET_IP   = 192.168.0.195
TARGET_USER = root
TARGET      = gnublin-step




all: 
	arm-linux-gnueabi-g++ -c ../../gnublin.cpp
	arm-linux-gnueabi-g++ -c $(TARGET).cpp
	arm-linux-gnueabi-g++ -o $(TARGET) $(TARGET).o gnublin.o 
	
clean:
	rm -f $(TARGET)
	rm  -f *.o

install: all
	scp $(TARGET) $(TARGET_USER)@$(TARGET_IP):~/
	

