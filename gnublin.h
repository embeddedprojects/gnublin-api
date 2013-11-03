//********************************************
//GNUBLIN API -- HEADER FILE
//build date: 11/03/13 18:42
//******************************************** 


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
        int fd;
        int errorMsg(std::string message);
        int open_fd();
        void close_fd();
	void init(std::string DeviceFile, int Address);
public:
	gnublin_i2c();
	gnublin_i2c(int Address);
	gnublin_i2c(std::string DeviceFile, int Address);
	~gnublin_i2c();
	bool fail();
	int setAddress(int Address);
	int getAddress();
	const char *getErrorMessage();
	int setDevicefile(std::string filename);
	int receive(unsigned char *RxBuf, int length);
	int receive(unsigned char RegisterAddress, unsigned char *RxBuf, int length);
	int send(unsigned char *TxBuf, int length);
	int send(unsigned char RegisterAddress, unsigned char *TxBuf, int length);
	int send(unsigned char value);
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
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
//*******************************************************************
//Class for accessing GNUBLIN serial device
//*******************************************************************
/**
* @class gnublin_serial
* @~english
* @brief Class for accessing GNUBLIN serial device
*
* The GNUBLIN serial interface can easily accessed with this class
* @~german 
* @brief Klasse für den zugriff auf die serielle Schnittstelle des GNUBLIN
*
* Die GNUBLIN I2C Klasse gewährt einfachen Zugriff auf die serielle schnittstelle des GNUBLIN
*/

class gnublin_serial {
	struct termios  config;
	bool error_flag;
	std::string devicefile;
	long baudrate;
	std::string ErrorMessage;
        int fd;
        int errorMsg(std::string message);
        int open_fd();
        void close_fd();
	void init(std::string DeviceFile, int rate);
public:
	gnublin_serial();
	gnublin_serial(std::string Devicefile);
	gnublin_serial(std::string Devicefile, int rate);
	bool fail();
	int send(unsigned char *TxBuf, int length);
	int setDevicefile(std::string filename);
	int setBaudrate(int rate);

};
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

//***************************************************************************
// Class for creating pwm signals
//***************************************************************************

/**
* @class gnublin_pwm
* @~english
* @brief Class for creating pwm signals
*
* This class manages the duty-cycle and the clock divider of a pwm signal.
* @~german
* @brief Klasse zum Erstellen von pwm Signalen
*
* Diese Klasse ermöglicht das Einstellen des Duty-Cycle und das Einstellen eines Clock Teilers.
*/

using namespace std;

class gnublin_pwm{
  public:
    gnublin_pwm();
		void setValue(float v);
		void setClock(int num);
};





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
		int readState(int pin);
		int writePort(int port, unsigned char value);
		unsigned char readPort(int port);
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
	gnublin_i2c i2c;
	bool error_flag;
	std::string ErrorMessage;
public:
	gnublin_module_relay();
	const char *getErrorMessage();
	bool fail();
	void setAddress(int Address);
	void setDevicefile(std::string filename);
	int switchPin(int pin, int value);
	int readState(int pin);
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
    unsigned int accshape;
    unsigned int stepmode;
    std::string ErrorMessage;
  public:
    gnublin_module_step();
    void setAddress(int Address);
    void setDevicefile(std::string filename);
    int setIrun(unsigned int newIrun);
    int setIhold(unsigned int newIhold);
    int setVmax(unsigned int newVmax);
    int setVmin(unsigned int newVmin);
    int setStepMode(unsigned int newStepMode);
    int setAccShape(unsigned int newAccShape);
    int writeTMC(unsigned char* buffer, int num);
    int readTMC(unsigned char* RxBuf, int num);
    int burnNewAddress(int new_address);
    int getFullStatus1();
    int getFullStatus2();
    int runInit();
    int setMotorParam();
    int setMotorParam(unsigned int newIrun, unsigned int newIhold, unsigned int newVmax, unsigned int newVmin, unsigned int newAccShape, unsigned int newStepMode);
    int hardStop();
    int softStop();
    int resetPosition();
    int setPosition(int position);
    int getSwitch();
    int getActualPosition();
    int drive(int steps);
    int getMotionStatus();
    const char* getErrorMessage();
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
//***** NEW BLOCK *****

//*******************************************************************
//Class for accessing the MCP4728 digital to analog IC via I2C
//*******************************************************************
/**
* @class gnublin_module_dac
* @~english
* @brief Class for accessing the MCP4728 digital to analog IC via I2C
*
* @~german 
* @brief Klasse für den zugriff auf den MCP4728 digital zu analog Wandler 
*
*/
class gnublin_module_dac {
private:
	bool error_flag;
	gnublin_i2c i2c;
	int _channel[4];
	int _gain[4];
	int _vRef[4];
	std::string ErrorMessage;
public:
	gnublin_module_dac();
	const char *getErrorMessage();
	bool fail();
	void setAddress(int Address);
	int read(int);
	void gain(int channel, int val);
	void gainEeprom(int channel, int val);	
	void vRef(int channel, int val);
	void vRefEeprom(int channel, int val);
	void writeAll(int val_0, int val_1, int val_2, int val_3);
	void write(int channel, int value);										
	void writeEeprom(int channel, int value);
};
#include <string>

#ifndef _BASE64_H_
#define _BASE64_H_

std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_decode(std::string const& s);

#endif
// CSmtp.h: interface for the Smtp class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#ifndef __CSMTP_H__
#define __CSMTP_H__


#include <vector>
#include <string.h>
#include <assert.h>


#include <sys/types.h>
#include <sys/socket.h>
//#include <sys/ioctl.h>
#include <sys/unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
//#include <stdio.h>
#include <iostream>

#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
	
typedef unsigned short WORD;
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct hostent* LPHOSTENT;
typedef struct servent* LPSERVENT;
typedef struct in_addr* LPIN_ADDR;
typedef struct sockaddr* LPSOCKADDR;

#define TIME_IN_SEC		10		// how long client will wait for server response in non-blocking mode
#define BUFFER_SIZE 10240	  // SendData and RecvData buffers sizes
#define MSG_SIZE_IN_MB 5		// the maximum size of the message with all attachments
#define COUNTER_VALUE	100		// how many times program will try to receive data

const char BOUNDARY_TEXT[] = "__MESSAGE__ID__54yg6f6h6y456345";

enum CSmptXPriority
{
	XPRIORITY_HIGH = 2,
	XPRIORITY_NORMAL = 3,
	XPRIORITY_LOW = 4
};


/**
* @class gnublin_smtp
* @~english
* @brief Class for sending mails.
*
* This class manages to send mails.
* @~german
* @brief Klasse um Mails zu versenden.
*
* Diese Klasse ermöglicht das Senden und Empfangen von E-Mails.
*/
class gnublin_smtp  
{
public:
	gnublin_smtp();
	virtual ~gnublin_smtp();
	void AddRecipient(const char *email, const char *name=NULL);
	void AddBCCRecipient(const char *email, const char *name=NULL);
	void AddCCRecipient(const char *email, const char *name=NULL);    
	void AddAttachment(const char *path);   
	void AddMsgLine(const char* text);
	void DelRecipients(void);
	void DelBCCRecipients(void);
	void DelCCRecipients(void);
	void DelAttachments(void);
	void DelMsgLines(void);
	void DelMsgLine(unsigned int line);
	void ModMsgLine(unsigned int line,const char* text);
	unsigned int GetBCCRecipientCount() const;    
	unsigned int GetCCRecipientCount() const;
	unsigned int GetRecipientCount() const;    
//	const char* GetLocalHostIP() const;
	const char* GetLocalHostName() const;
	const char* GetMsgLineText(unsigned int line) const;
	unsigned int GetMsgLines(void) const;
	const char* GetReplyTo() const;
	const char* GetMailFrom() const;
	const char* GetSenderName() const;
	const char* GetSubject() const;
	const char* GetXMailer() const;
	CSmptXPriority GetXPriority() const;
	void Send();
	void SetSubject(const char*);
	void SetSenderName(const char*);
	void SetSenderMail(const char*);
	void SetReplyTo(const char*);
	void SetXMailer(const char*);
	void SetLogin(const char*);
	void SetPassword(const char*);
	void SetXPriority(CSmptXPriority);
	void SetSMTPServer(const char* server,const unsigned short port=0);
	
	//Error handling
	enum CSmtpError
	{
		CSMTP_NO_ERROR = 0,
		WSA_STARTUP = 100, // WSAGetLastError()
		WSA_VER,
		WSA_SEND,
		WSA_RECV,
		WSA_CONNECT,
		WSA_GETHOSTBY_NAME_ADDR,
		WSA_INVALID_SOCKET,
		WSA_HOSTNAME,
		WSA_IOCTLSOCKET,
		WSA_SELECT,
		BAD_IPV4_ADDR,
		UNDEF_MSG_HEADER = 200,
		UNDEF_MAIL_FROM,
		UNDEF_SUBJECT,
		UNDEF_RECIPIENTS,
		UNDEF_LOGIN,
		UNDEF_PASSWORD,
		UNDEF_RECIPIENT_MAIL,
		COMMAND_MAIL_FROM = 300,
		COMMAND_EHLO,
		COMMAND_AUTH_LOGIN,
		COMMAND_DATA,
		COMMAND_QUIT,
		COMMAND_RCPT_TO,
		MSG_BODY_ERROR,
		CONNECTION_CLOSED = 400, // by server
		SERVER_NOT_READY, // remote server
		SERVER_NOT_RESPONDING,
		SELECT_TIMEOUT,
		FILE_NOT_EXIST,
		MSG_TOO_BIG,
		BAD_LOGIN_PASS,
		UNDEF_XYZ_RESPONSE,
		LACK_OF_MEMORY,
		TIME_ERROR,
		RECVBUF_IS_EMPTY,
		SENDBUF_IS_EMPTY,
		OUT_OF_MSG_RANGE,
	};
	gnublin_smtp (CSmtpError err_) : ErrorCode(err_) {}
	CSmtpError GetErrorNum(void) const {return ErrorCode;}
	std::string GetErrorText(void) const;





private:	

	CSmtpError ErrorCode;


	std::string m_sLocalHostName;
	std::string m_sMailFrom;
	std::string m_sNameFrom;
	std::string m_sSubject;
	std::string m_sXMailer;
	std::string m_sReplyTo;
	std::string m_sIPAddr;
	std::string m_sLogin;
	std::string m_sPassword;
	std::string m_sSMTPSrvName;
	unsigned short m_iSMTPSrvPort;
	CSmptXPriority m_iXPriority;
	char *SendBuf;
	char *RecvBuf;
	
	SOCKET hSocket;

	struct Recipient
	{
		std::string Name;
		std::string Mail;
	};

	std::vector<Recipient> Recipients;
	std::vector<Recipient> CCRecipients;
	std::vector<Recipient> BCCRecipients;
	std::vector<std::string> Attachments;
	std::vector<std::string> MsgBody;
 
	void ReceiveData();
	void SendData();
	void FormatHeader(char*);
	int SmtpXYZdigits();
	SOCKET ConnectRemoteServer(const char* server, const unsigned short port=0);
	
};


#endif // __CSMTP_H__
#include <fstream>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <stdarg.h>
#include <sstream>

//***************************************************************************
// Class for creating csv files
//***************************************************************************

/**
* @class gnublin_csv
* @~english
* @brief Class for creating csv files
*
* This class manages creating of csv files und writing content to them.
* @~german
* @brief Klasse zum Erstellen von csv Dateien
*
* Diese Klasse ermöglicht das Erstellen der Dateien sowie das Schreiben von Inhalten.
*/

using namespace std;

class gnublin_csv{
  public:
    gnublin_csv();
		gnublin_csv(string new_filename);
  	int open(string new_filename);
		int addRow(int quantity, ...);
		void close();
		void delimiterRow(string delimiterSign);
	  void delimiterColumn(char delimiterSign);
  	void delimiterField(char delimiterSign);
		void delimiterField();
		template <typename T>
		string NumberToString(T Number) {
  		string str;
			stringstream ss;
  		ss << Number;
			str = ss.str();
  		return ss.str();
		}



	private:
		string filename;
		bool user_file_handle;
		string delimiterRowSign;
		char delimiterColumnSign;
		char delimiterFieldSign;
};



#endif
