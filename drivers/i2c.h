#include "../include/includes.h"
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
	bool close_mode;
	int slave_address;
	int fd;
	std::string devicefile;
	std::string ErrorMessage;
public:
	gnublin_i2c();
	bool fail();
	void setAddress(int Address);
	int getAddress();
	const char *getErrorMessage();
	void setDevicefile(std::string filename);
	int openDevFile();
	void closeDevFile();
	void setclosemode(int mode);
	int receive(unsigned char *RxBuf, int length);
	int receive(unsigned char RegisterAddress, unsigned char *RxBuf, int length);
	int send(unsigned char *TxBuf, int length);
	int send(unsigned char RegisterAddress, unsigned char *TxBuf, int length);
	int send(int value);
};
