#Compiler and Compiler-Flags
CXX = arm-linux-gnueabi-g++ 
CXXFLAGS = -Wall

#Define which Board you want: 
#GNUBLIN
#RASPBERRY_PI

BOARD :=  GNUBLIN

#DO NOT EDIT BEYOND THIS LINE!
BOARDDEF := -DBOARD=$(BOARD)

