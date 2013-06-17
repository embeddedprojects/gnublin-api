#include "../include/includes.h"
#include "driver.h"
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

class gnublin_i2c : public gnublin_driver {
	int slave_address;
	std::string devicefile;
	int fd;
	int open_fd();
	void close_fd();
	void init(std::string DeviceFile, int Address);
        void onError();
public:
	gnublin_i2c();
	gnublin_i2c(int Address);
	gnublin_i2c(std::string DeviceFile, int Address);
	~gnublin_i2c();
	int setAddress(int Address);
	int getAddress();
	int setDevicefile(std::string filename);
	int receive(unsigned char *RxBuf, int length);
	int receive(unsigned char RegisterAddress, unsigned char *RxBuf, int length);
	int send(unsigned char *TxBuf, int length);
	int send(unsigned char RegisterAddress, unsigned char *TxBuf, int length);
	int send(unsigned char value);
};
