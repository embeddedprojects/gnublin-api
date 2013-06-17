#include "../include/includes.h"
#include "driver.h"
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
class gnublin_spi : gnublin_driver {
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
	private:
		int fd;
		void onError() {};	
};
