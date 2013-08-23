
/** @~english
 * @mainpage Gnublin-API 
 *  
 *  @section sec1 Introduction 
 *  Many expansion boards with matching small console programs and a common API allow you to realize little applications or controls very fast. You look for the appropriate modules and test it with a little terminal tool. When you know how your first program should look like, you can easily write it by using the API. 
 *
 * <p><img src="http://wiki.gnublin.org/images/2/24/RASPBERRY-BRIDGE_Aufbau.JPG" ></p>
 *
 * <ul>
 * 	<li> <a href="http://en.gnublin.org/index.php/Main_Page"> Gnublin-Wiki </a></li>
 *	<li> <a href="http://en.gnublin.org/index.php/API"> API-Wiki </a></li> 
 *	<li> <a href="http://forum.gnublin.org/index.php?module=content&action=show&page=hardware&lang=en"> Gnublin-Forum </a></li> 
 *	<li> <a href="http://shop.embedded-projects.net/index.php?module=artikel&action=gruppe&id=62&lang=en"> Gnublin-Shop </a></li> 
 * </ul>
 * @~german
 * @mainpage Gnublin-API 
 *  
 *  @section sec1 Einleitung 
 *  Viele Erweierungsboards mit passenden kleinen Konsolenprogrammen und einer gemeinsamen API ermöglichen einem sehr schnell Anwendungen oder Steuerungen zu realisieren. Man sucht sicht die passenden Module aus, testet diese schnell und einfach mit kleinen Tools auf der Kommandozeile. Ist man sich im klaren, wie die erste Anwendung aussehen soll kann man diese einfach mit Hilfe der API umsetzten. 
 *
 * <p><img src="http://wiki.gnublin.org/images/2/24/RASPBERRY-BRIDGE_Aufbau.JPG" ></p>
 *
 * <ul>
 * 	<li> <a href="http://wiki.gnublin.org/index.php/Hauptseite"> Gnublin-Wiki </a></li>
 *	<li> <a href="http://wiki.gnublin.org/index.php/API"> API-Wiki </a></li> 
 *	<li> <a href="http://forum.gnublin.org/"> Gnublin-Forum </a></li> 
 *	<li> <a href="http://shop.embedded-projects.net/index.php?module=artikel&action=gruppe&id=62"> Gnublin-Shop </a></li> 
 * </ul>
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
#define BEAGLEBONE_BLACK 3

#define OUTPUT 	"out"
#define INPUT	"in"
#define LOW	0
#define HIGH	1
#define ON	1
#define OFF	0
#define IN	1
#define OUT	0

#endif
