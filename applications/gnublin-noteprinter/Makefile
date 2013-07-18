CXX:= arm-linux-gnueabi-g++
EXECUTABLE:= noteprinter
TargetIP:= 192.168.0.247
SOURCES:=main.cpp keyboard.cpp application.cpp screens.cpp display_buffer.cpp display.cpp note.cpp gnublin.cpp
OBJECTS:=$(SOURCES:.cpp=.o)

all: $(OBJECTS)
	$(CXX) -o $(EXECUTABLE) $(OBJECTS) -lusb

%.o: %.cpp %.h
	$(CXX) -c $<

rebuild: $(SOURCES)
	$(CXX) -o $(EXECUTABLE) $(SOURCES) -lusb

install: all
	scp $(EXECUTABLE) root@$(TargetIP):
clean:
	rm -rf *.o $(EXECUTABLE)
