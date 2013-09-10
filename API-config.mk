#Crosscompiler for Gnublin
#CXX := arm-linux-gnueabi-g++ 
#Crosscompiler for Raspberry Pi:
#CXX := arm-linux-gnueabihf-g++
#Compiler for onboard compilation:
CXX := g++

#Compilerflags:
CXXFLAGS = -Wall -O

#Architecture for gnublin:
#Architecture = armel
#Architecture for raspberryPi an beaglebone black:
Architecture = armhf

#Define which Board you want: 
#BOARD := GNUBLIN
BOARD := RASPBERRY_PI
#BOARD := BEAGLEBONE_BLACK

#DO NOT EDIT BEYOND THIS LINE!
BOARDDEF := -DBOARD=$(BOARD)

