#include "../include/includes.h"


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
