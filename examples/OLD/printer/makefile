CC          = arm-linux-gnueabi-g++
TARGET_IP   = 192.168.0.195
TARGET_USER = root
TARGET      = printer




all: 
	arm-linux-gnueabi-g++ -c ../../gnublin.cpp
	arm-linux-gnueabi-g++ -c printer.cpp
	arm-linux-gnueabi-g++ -c printer_temp.cpp
	arm-linux-gnueabi-g++ -o printer printer.o gnublin.o 
	arm-linux-gnueabi-g++ -o printer_temp printer_temp.o gnublin.o 
	
clean:
	rm $(OBJ) -f $(TARGET)
	rm  -f *.o

install: all
	scp printer $(TARGET_USER)@$(TARGET_IP):/usr/bin
	scp printer_temp $(TARGET_USER)@$(TARGET_IP):/usr/bin
	
install_temp: all
	scp printer_temp $(TARGET_USER)@$(TARGET_IP):/usr/bin
	
install_printer: all
	scp printer $(TARGET_USER)@$(TARGET_IP):/usr/bin
