
/** @mainpage Gnublin-API 
 *  
 *  @section sec1 Einleitung 
 *  Viele Erweierungsboards mit passenden kleinen Konsolenprogrammen und einer gemeinsamen API ermöglichen einem sehr schnell Anwendungen oder Steuerungen zu realisieren. Man sucht sicht die passenden Module aus, testet diese schnell und einfach mit kleinen Tools auf der Kommandozeile. Ist man sich im klaren, wie die erste Anwendung aussehen soll kann man diese einfach mit Hilfe der API umsetzten. 
 
 * <p><img src="http://wiki.gnublin.org/images/2/24/RASPBERRY-BRIDGE_Aufbau.JPG" ></p>
 */
#ifndef INCLUDE_FILE
#define INCLUDE_FILE

#include <cstring>
#include <ctime>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <linux/spi/spidev.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

#include "functions.h"

//BOARDS
#define GNUBLIN 1
#define RASPBERRY_PI 2

#define OUTPUT 	"out"
#define INPUT	"in"
#define LOW	0
#define HIGH	1
#define ON	1
#define OFF	0
#define IN	1
#define OUT	0

#endif
