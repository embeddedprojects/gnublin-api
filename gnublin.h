//********************************************
//GNUBLIN API -- HEADER FILE
//build date: 05/08/13 10:27
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

int stringToNumber(std::string str);
std::string numberToString(int num);
int hexstringToNumber(std::string str);
//***** NEW BLOCK *****

/**
* @class gnublin_gpio
* @~english
* @brief Class for accessing GNUBLIN GPIO-Ports
*
* With the gnublin_gpio API you can controll the GPIO-Ports of the GNUBLIN Board
* @~german 
* @brief Klasse für den zugriff auf die GPIO Pins
*
* Mit der gnublin_gpio API lassen sich die GPIO-Ports auf dem GNUBLIN einfach aus dem eigenem Programm heraus ansteuern.  
*/
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
/**
* @class gnublin_i2c
* @~english
* @brief Class for accessing GNUBLIN i2c bus
*
* The GNUBLIN I2C bus can easily accessed with this class
* @~german 
* @brief Klasse für den zugriff auf den GNUBLIN I2C Bus
*
* Die GNUBLIN I2C Klasse gewährt einfachen Zugriff auf den I2C Bus
*/ 

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
	int receive(unsigned char *RxBuf, int length);
	int receive(unsigned char RegisterAddress, unsigned char *RxBuf, int length);
	int send(unsigned char *TxBuf, int length);
	int send(unsigned char RegisterAddress, unsigned char *TxBuf, int length);
	int send(int value);
};
//***** NEW BLOCK *****

//***************************************************************************
// Class for accessing the SPI-Bus
//***************************************************************************

/**
* @class gnublin_spi
* @~english
* @brief Class for accesing the SPI-Bus.
*
* This class manages the sendnd and reciving of Data via the SPI-Bus.
* @~german
* @brief Klasse für den Zugriff auf den SPI-Bus.
*
* Diese Klasse ermöglicht das Senden und Empfangen von Daten über den SPI-Bus.
*/
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

#if (BOARD != RASPBERRY_PI)
//****************************************************************************
// Class for easy acces to the GPAs
//****************************************************************************
/**
* @class gnublin_adc
* @~english
* @brief Class for easy acces to the GPAs
*
* With the gnublin_adc API you can access the GPAs of the GNUBLIN Board
* @~german 
* @brief Klasse für den zugriff auf die GPAs
*
* Mit der gnublin_adc API lassen sich die GPAs auf dem GNUBLIN einfach aus dem eigenem Programm heraus auslesen.  
*/
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

#endif
//***** NEW BLOCK *****

//***************************************************************************
// Class for accesing the GNUBLIN MODULE-DISPLAY 2x16
//***************************************************************************

/**
* @class gnublin_module_dogm
* @~english
* @brief Class for accesing the GNUBLIN MODULE-DISPLAY 2x16
*
* This class manages to display datas on the DOGM-Display.
* @~german
* @brief Klasse für den Zugriff auf das GNUBLIN MODULE-DISPLAY 2x16.
*
* Diese Klasse ermöglicht das Beschrieben des DOGM-Displays.
*/
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
//*******************************************************************
//Class for accessing the LM75 IC via I2C
//*******************************************************************
/**
* @class gnublin_module_lm75
* @~english
* @brief Class for accessing the LM75 IC via I2C
*
* @~german 
* @brief Klasse für den zugriff auf den LM75 IC via I2C Bus
*
*/
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

//*****************************************************************************
// Class for accesing GNUBLIN Module-ADC / ADS7830
//*****************************************************************************

/**
* @class gnublin_module_adc
* @~english
* @brief Class for accesing the GNUBLIN MODULE-ADC
*
* This class manages to use the GNUBLIN MODULE-ADC
* @~german
* @brief Klasse für den Zugriff auf das GNUBLIN MODULE-ADC
*
* Mit dieser Klasse kann auf das GNUBLIN MODULE-ADC zugegriffen werden.
*/
class gnublin_module_adc {
	public:
		gnublin_module_adc();
		int setAddress(int adr);
		int setDevicefile(std::string file);
		int setReference(int value);
		int getValue(int channel);
		int getValue(int channel1, int channel2);
		int getVoltage(int channel);
		int getVoltage(int channel1, int channel2);
		bool fail();
		const char *getErrorMessage();
	private:
		gnublin_i2c i2c;
		bool error_flag;
		std::string ErrorMessage;
		int reference_flag; // (1 = intern, 0 extern)
		int referenceValue;
};
//***** NEW BLOCK *****

//*******************************************************************
//Class for accessing GNUBLIN Module-Portexpander or any PCA9555
//*******************************************************************
/**
* @class gnublin_module_pca9555
* @~english
* @brief Class for accessing the PCA9555 IC via I2C
*
* @~german 
* @brief Klasse für den zugriff auf den PCA9555 IC via I2C Bus
*
*/
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
		int writePort(int port, unsigned char value);
};
//***** NEW BLOCK *****


//****************************************************************************
// Class for easy use of the GNUBLIN Module-Relay
//****************************************************************************
/**
* @class gnublin_module_relay
* @~english
* @brief Class for accessing GNUBLIN module-relay
*
* The GNUBLIN Module-relay can be easily controlled with the gnublin_step API. The Module uses the I2C-Bus.
* @~german 
* @brief Klasse für den zugriff auf das GNUBLIN module-relay
*
* Das GNUBLIN module-relay lässt sich mit Hilfe der gnublin_relay API ganz einfach ansteuern. Das Modul nutzt die I2C-Schnittstelle.  
*/ 
class gnublin_module_relay {
	gnublin_module_pca9555 pca9555;
	bool error_flag;
	std::string ErrorMessage;
public:
	gnublin_module_relay();
	const char *getErrorMessage();
	bool fail();
	void setAddress(int Address);
	void setDevicefile(std::string filename);
	int switchPin(int pin, int value);
};

/**
* @class gnublin_module_step
* @~english
* @brief Class for accessing GNUBLIN module-step
*
* The GNUBLIN Module-step can be easily controlled with the gnublin_step API. The Module uses the I2C-Bus.
* @~german 
* @brief Klasse für den zugriff auf das GNUBLIN module-step
*
* Das GNUBLIN module-step lässt sich mit Hilfe der gnublin_step API ganz einfach ansteuern. Das Modul nutzt die I2C-Schnittstelle.  
*/ 
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
	bool fail();
	int setIrun(unsigned int newIrun);
	int setIhold(unsigned int newIhold);
	int setVmax(unsigned int newVmax);
	int setVmin(unsigned int newVmin);
	int writeTMC(unsigned char *buffer, int num);
	int readTMC(unsigned char *RxBuf, int num);
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
////////////////////////////////////////////////////////////////////////////////
//connection on the Portexpander Port 0
#define LCD_EN			0x04
#define LCD_RS			0x01
#define LCD_RW			0x02
////////////////////////////////////////////////////////////////////////////////
// LCD Timings
#define LCD_BOOTUP_MS           15000
#define LCD_ENABLE_US           20
#define LCD_WRITEDATA_US        46
#define LCD_COMMAND_US          42

#define LCD_SOFT_RESET_MS       5000
#define LCD_SET_8BITMODE_MS     5000

#define LCD_CLEAR_DISPLAY_MS    2000
#define LCD_CURSOR_HOME_MS      2000

////////////////////////////////////////////////////////////////////////////////
//definitions of the lines of an 4x20 Display

#define LCD_DDADR_LINE1         0x00
#define LCD_DDADR_LINE2         0x40
#define LCD_DDADR_LINE3         0x14
#define LCD_DDADR_LINE4         0x54

////////////////////////////////////////////////////////////////////////////////
// LCD instructions and arguments
// to use with lcd_command

// Clear Display -------------- 0b00000001
#define LCD_CLEAR_DISPLAY       0x01

// Cursor Home ---------------- 0b0000001x
#define LCD_CURSOR_HOME         0x02

// Set Entry Mode ------------- 0b000001xx
#define LCD_SET_ENTRY           0x04

#define LCD_ENTRY_DECREASE      0x00
#define LCD_ENTRY_INCREASE      0x02
#define LCD_ENTRY_NOSHIFT       0x00
#define LCD_ENTRY_SHIFT         0x01

// Set Display ---------------- 0b00001xxx
#define LCD_SET_DISPLAY         0x08

#define LCD_DISPLAY_OFF         0x00
#define LCD_DISPLAY_ON          0x04
#define LCD_CURSOR_OFF          0x00
#define LCD_CURSOR_ON           0x02
#define LCD_BLINKING_OFF        0x00
#define LCD_BLINKING_ON         0x01

// Set Shift ------------------ 0b0001xxxx
#define LCD_SET_SHIFT           0x10

#define LCD_CURSOR_MOVE         0x00
#define LCD_DISPLAY_SHIFT       0x08
#define LCD_SHIFT_LEFT          0x00
#define LCD_SHIFT_RIGHT         0x04

// Set Function --------------- 0b001xxxxx
#define LCD_SET_FUNCTION        0x20

#define LCD_FUNCTION_4BIT       0x00
#define LCD_FUNCTION_8BIT       0x10
#define LCD_FUNCTION_1LINE      0x00
#define LCD_FUNCTION_2LINE      0x08
#define LCD_FUNCTION_5X7        0x00
#define LCD_FUNCTION_5X10       0x04

#define LCD_SOFT_RESET          0x30

// Set DD RAM Address --------- 0b1xxxxxxx  (Display Data RAM)
#define LCD_SET_DDADR           0x80



/**
* @class gnublin_module_lcd
* @~english
* @brief Class for accessing GNUBLIN Module-LCD 4x20
*
* The GNUBLIN Module-LCD_4x20 can be easily controlled with the gnublin_lcd API. The Display uses the I2C-Bus.
* @~german 
* @brief Klasse für den zugriff auf das GNUBLIN Module-LCD 4x20
*
* Das GNUBLIN Module-LCD_4x20 lässt sich mit Hilfe der gnublin_lcd API ganz einfach ansteuern. Das Display nutzt die I2C-Schnittstelle.  
*/ 
class gnublin_module_lcd {
		bool error_flag;
		gnublin_module_pca9555 pca;
		std::string ErrorMessage;
		const char *version;
public:
		gnublin_module_lcd();
		const char *getErrorMessage();
		bool fail();
		void setAddress(int Address);
		void setDevicefile(std::string filename);
		int out(unsigned char rsrw, unsigned char data );
		int sendData(unsigned char data);
		int command(unsigned char data);
		int clear();
		int home();
		int setdisplay(int cursor, int blink);
		int setcursor(unsigned char x, unsigned char y);
		int string(const char *data);
		int init();
		
};
#endif
