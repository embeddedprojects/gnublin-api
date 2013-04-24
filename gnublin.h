//********************************************
//GNUBLIN API -- HEADER FILE
//build date: 04/23/13 14:08
//******************************************** 

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



#define OUTPUT 	"out"
#define INPUT	"in"
#define LOW	0
#define HIGH	1
#define ON	1
#define OFF	0

 
int stringToNumber(std::string str);
std::string numberToString(int num);
int hexstringToNumber(std::string str);
//***** NEW BLOCK *****

//****************************************************************************
// Class for easy acces to the GPIOs
//****************************************************************************

class gnublin_gpio {
	public:
		gnublin_gpio();
		bool fail();
		int pinMode(int pin, std::string direction); //Defines GPIO<pin> as INPUT or OUTPUT
		int digitalWrite(int pin, int value); //Writes value on GPIO
		int digitalRead(int pin); //Reads value from GPIO<pin>
		int unexport(int pin);
		const char *getErrorMessage();
	private:
		bool error_flag;
		std::string ErrorMessage;
};
//***** NEW BLOCK *****
//*******************************************************************
//Class for accessing GNUBLIN i2c Bus
//*******************************************************************

class gnublin_i2c {
	bool error_flag;
	int slave_address;
	std::string devicefile;
	std::string ErrorMessage;
public:
	gnublin_i2c();
	bool fail();
	void setAddress(int Address);
	int getAddress();
	const char *getErrorMessage();
	void setDevicefile(std::string filename);
	int receive(char *RxBuf, int length);
	int receive(unsigned char RegisterAddress, char *RxBuf, int length);
	int send(char *TxBuf, int length);
	int send(unsigned char RegisterAddress, char *TxBuf, int length);
	int send(int value);
};
//***** NEW BLOCK *****


//***************************************************************************
// Class for accessing the SPI-Bus
//***************************************************************************

class gnublin_spi{
	public:
		gnublin_spi();
		~gnublin_spi();
		int setMode(unsigned char mode);
		int getMode();
		int setLSB(unsigned char lsb);
		int getLSB();
		int setLength(unsigned char bits);
		int getLength();
		int setSpeed(unsigned int speed);
		int getSpeed();
		int receive(char* buffer, int len);
		int send(unsigned char* tx, int length);
		int setCS(int cs);
		int message(unsigned char* tx, int tx_length, unsigned char* rx, int rx_length);
		const char *getErrorMessage();
		bool fail();
	private:
		int fd;
		bool error_flag;
		std::string ErrorMessage;
		
};
//***** NEW BLOCK *****


//****************************************************************************
// Class for easy acces to the GPAs
//****************************************************************************

class gnublin_adc {
	public:
		gnublin_adc();
		int getValue(int pin);
		int getVoltage(int pin);
		int setReference(int ref);
		bool fail();
		const char *getErrorMessage();
	private:
		bool error_flag;
		std::string ErrorMessage;
};
//***** NEW BLOCK *****

//***************************************************************************
// Class for accesing the GNUBLIN MODULE-DISPLAY 2x16
//***************************************************************************

class gnublin_module_dogm{
	public:
		gnublin_module_dogm();
		int init();
		bool fail();
		const char *getErrorMessage();
		int setRsPin(int pin);
		int setCS(int cs);
		int print(char* buffer);
		int print(char* buffer, int line);
		int print(char* buffer, int line, int off);
		int offset(int num);
		int clear();
		int returnHome();
		int shift(int num);
		int controlDisplay(int power, int cursor, int blink);
	private:
		gnublin_spi spi;
		gnublin_gpio gpio;
		bool error_flag;
		bool init_flag;
		int rs_pin;
		std::string ErrorMessage;

};
//***** NEW BLOCK *****


class gnublin_module_lm75 {
	bool error_flag;
	gnublin_i2c i2c;
	std::string ErrorMessage;
public:
	gnublin_module_lm75();
	const char *getErrorMessage();
	bool fail();
	void setAddress(int Address);
	void setDevicefile(std::string filename);
	int getTemp();
	float getTempFloat();
	short getValue();
};
//***** NEW BLOCK *****


//Hinweis: evtl. Fehlerhaft nicht getestet
//*****************************************************************************
// Class for accesing GNUBLIN Module-ADC / ADS7830
//*****************************************************************************

class gnublin_module_adc {
	public:
		gnublin_module_adc();
		gnublin_module_adc(int adr);
		gnublin_module_adc(int bus, int adr);
		int getValue(int channel);
		const char *getErrorMessage();
	private:
		std::string adress;
		std::string i2cbus;
		std::string ErrorMessage;
};
//***** NEW BLOCK *****

//*******************************************************************
//Class for accessing GNUBLIN Module-Portexpander or any PCA9555
//*******************************************************************

class gnublin_module_pca9555 {
		bool error_flag;
		gnublin_i2c i2c;
		std::string ErrorMessage;
public:
		gnublin_module_pca9555();
		const char *getErrorMessage();
		bool fail();
		void setAddress(int Address);
		void setDevicefile(std::string filename);
		int pinMode(int pin, std::string direction);
		int portMode(int port, std::string direction);
		int digitalWrite(int pin, int value);
		int digitalRead(int pin);		
};
//***** NEW BLOCK *****


class gnublin_module_step {
	gnublin_i2c i2c;

	unsigned int irun;
	unsigned int ihold;
	unsigned int vmax;
	unsigned int vmin;
	std::string ErrorMessage;
public:
	gnublin_module_step();
	void setAddress(int Address);
	void setDevicefile(std::string filename);
	int setIrun(unsigned int newIrun);
	int setIhold(unsigned int newIhold);
	int setVmax(unsigned int newVmax);
	int setVmin(unsigned int newVmin);
	int writeTMC(char *buffer, int num);
	int readTMC(char *RxBuf, int num);
	int burnNewAddress(int new_address);
	int getFullStatus1();
	int getFullStatus2();
	int runInit();
	int setMotorParam();
	int setMotorParam(unsigned int newIrun, unsigned int newIhold, unsigned int newVmax, unsigned int newVmin);
	int hardStop();
	int softStop();
	int resetPosition();
	int setPosition(int position);
	int getSwitch();
	int getActualPosition();
	int drive(int steps);
	int getMotionStatus();
	const char *getErrorMessage();
};
#endif
