#include "../include/includes.h"
#include "../include/functions.h"
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
