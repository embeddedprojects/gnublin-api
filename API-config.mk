#Crosscompiler for Gnublin
CXX := arm-linux-gnueabi-g++ 
#Crosscompiler for Raspberry Pi:
#CXX := arm-linux-gnueabihf-g++
#Compiler for onboard compilation:
#CXX := g++

#Compilerflags:
CXXFLAGS = -Wall

#Define which Board you want: 
#GNUBLIN
#RASPBERRY_PI

BOARD :=  GNUBLIN

#DO NOT EDIT BEYOND THIS LINE!
BOARDDEF := -DBOARD=$(BOARD)

