#include "spi.h"


//***************************************************************************
// Class for accessing the SPI-Bus
//***************************************************************************

/**
* @~english
* @brief Loads the standard SPI driver if necessary.
*
* Default chipselect:
* GNUBLIN: CS = 11
* RASPBERRY PI: CS = 0
* @~german
* @brief Die Standart SPI-Treiber werden geladen, wenn sie noch nicht vorhanden sind.
*
* Default chipselect:
* GNUBLIN: CS = 11
* RASPBERRY PI: CS = 0
*/
gnublin_spi::gnublin_spi(){
	error_flag = false;
	#if BOARD == RASPBERRY_PI
	std::string device = "/dev/spidev0.0";
	#else
	std::string device = "/dev/spidev0.11";
	#endif
	fd = open(device.c_str(), O_RDWR);
	if (fd < 0) {
		#if BOARD == RASPBERRY_PI
		system("modprobe spi-bcm2708");
		#else
		system("modprobe spidev cs_pin=11");
		#endif
		sleep(1);
		fd = open(device.c_str(), O_RDWR);
	}
}


//******************** destructor *******************************************

gnublin_spi::~gnublin_spi(){
	close(fd);
}


//******************** fail() ***********************************************
/**
* @~english
* @brief Returns the errorflag to detect error in the previous called method.
*
* @return error_flag
*
* @~german
* @brief Gibt das errorflag zurück, um Fehler in der zuvor aufgerugfenen Methode zu erkennen.
*
* @return error_flag
*/
bool gnublin_spi::fail(){
	return error_flag;
}


//-------------get Error Message-------------
/**
* @~english
* @brief Returns the ErrorMessage of the previous error if one exist.
*
* @return ErrorMessage as C-String
*
* @~german
* @brief Gibt die Fehlernachricht des zuvor aufgetretenen Fehlers zurück, wenn weine exisitert.
*
* @return ErrorMessage als C-String
*/
const char *gnublin_spi::getErrorMessage(){
	return ErrorMessage.c_str();
}


//*********************** setCS *********************************************

/**
* @~english
* @brief Set the custom chipselect pin
*
* @param Number of the chipselect-pin
* @return 1 by success, -1 by failure
*
* @~german
* @brief Setzt den benutzerdefinierten Chipselect-Pin.
*
* @param Nummer des Chipselect-Pin
* @return 1 bei Erfolg, -1 im Fehlerfall
*/
int gnublin_spi::setCS(int cs){
	std::string cs_str = numberToString(cs);
	std::string device = "/dev/spidev0." + cs_str;
	fd = open(device.c_str(), O_RDWR);
	if (fd < 0) {
		#if (BOARD == RASPBERRY_PI)
		std::string command = "modprobe spi-bcm2708 cs_pin=" + cs_str;
		#else
		std::string command = "modprobe spidev cs_pin=" + cs_str;
		#endif
		system(command.c_str());
		sleep(1);
		fd = open(device.c_str(), O_RDWR);
		if (fd < 0){
			error_flag = true;
			return -1;
		}
	}
	error_flag = false;
	return 1;
}


//******************** set Mode *********************************************

/**
* @~english
* @brief Set the SPI-mode
*
* @param Number of the SPI-Mode
* @return 1 by success, -1 by failure
*
* @~german
* @brief Setzt den SPI-Modus
*
* @param Nummer SPI-Modus
* @return 1 bei Erfolg, -1 im Fehlerfall
*/
int gnublin_spi::setMode(unsigned char mode){
	if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0){
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return 1;
}


//***************** getMode *************************************************

/**
* @~english
* @brief Returns the set SPI-Mode
*
* @return Number of the SPI-mode
*
* @~german
* @brief Gibt den eingestellten SPI-Modus zurück
*
* @return Nummer des SPI-Modus
*/
int gnublin_spi::getMode(){
	__u8 mode;
	if (ioctl(fd, SPI_IOC_RD_MODE, &mode) < 0){
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return mode;
}


//******************** setLSB ***********************************************

/**
* @~english
* @brief Set the LSB-mode
*
* @param 0: MSB first, 1 LSB first
* @return 1 by success, -1 by failure
*
* @~german
* @brief Setzt den LSB-Modus.
*
* @param 0: MSB zuerst; 1 LSB zuerst
* @return 1 bei Erfolg, -1 im Fehlerfall
*/
int gnublin_spi::setLSB(unsigned char lsb){
	if (ioctl(fd, SPI_IOC_WR_LSB_FIRST, &lsb) < 0){
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return 1;
}


//************************ getLSB() *****************************************

/**
* @~english
* @brief Returns the set LSB-Mode
*
* @return 0: MSB first, 1 LSB first
*
* @~german
* @brief Gibt den eingestellten LSB-Modus zurück.
*
* @return 0: MSB zuerst; 1 LSB zuerst
*/
int gnublin_spi::getLSB(){
	__u8 lsb;
	if (ioctl(fd, SPI_IOC_RD_LSB_FIRST, &lsb) < 0) {
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return lsb;
}


//*********************** setLength() ***************************************

/**
* @~english
* @brief Set the lenght of the words which will be send
*
* @param Number of bits of each word
* @return 1 by success, -1 by failure
*
* @~german
* @brief Legt die Länge der gesendeten Wörter fest
*
* @param Anzahl der Bits je Word
* @return 1 bei Erfolg, -1 im Fehlerfall
*/
int gnublin_spi::setLength(unsigned char bits){
	if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0){
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return 1;
}


//************************ getLength() **************************************

/**
* @~english
* @brief Returns the length of the words in bits
*
* @return Number of bits of each word
*
* @~german
* @brief Gibt die Anzahl von Bits je Wort zurück.
*
* @return anzahl der Bits je Wort
*/
int gnublin_spi::getLength(){
	__u8 bits;
	if (ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits) < 0){
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return bits;
}


//************************* setSpeed ****************************************
		* [int] -1  	for failure
/**
* @~english
* @brief Set the speed of the SPI-Bus
*
* @param Speed in Hz
* @return 1 by success, -1 by failure
*
* @~german
* @brief Legt die Geschwindigkeit des SPI-Buses fest.
*
* @param Geschwindigkeit in Hz
* @return 1 bei Erfolg, -1 im Fehlerfall
*/
int gnublin_spi::setSpeed(unsigned int speed){
	if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0){
		error_flag = true;
		return -1;
	}
	error_flag = true;
	return 1;
}


//************************* getSpeed() **************************************

/**
* @~english
* @brief Returns the speed of the SPI-Bus
*
* @return SPeed in Hz
*
* @~german
* @brief Gibt die Geschwindigkeit des SPI-Buses zurück.
*
* @return Geschwindigkeit in Hz
*/
int gnublin_spi::getSpeed(){
	__u32 speed;
	if (ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed) < 0){
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return speed;
}

//**************************** receive **************************************

/**
* @~english
* @brief Receive data from SPI Bus
*
* @param Buffer for recived datas
* @param Length of recived data
* @return 1 by success, -1 by failure
*
* @~german
* @brief Empfängt Daten über den SPI-Bus
*
* @param Buffer für die empfangenen Daten
* @param Anzahl der zu empfangenden Zeichen
* @return 1 bei Erfolg, -1 im Fehlerfall
*/
int gnublin_spi::receive(char* buffer, int len){
	if (read(fd, buffer, len) < 0) {
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return 1;
}

//*************************** send() ****************************************

/**
* @~english
* @brief Send data over the SPI Bus
*
* @param Datas which will be send
* @param Length of datas
* @return 1 by success, -1 by failure
*
* @~german
* @brief Sendet Daten über den SPI-Bus
*
* @param Zu sendende Daten
* @param Anzahl der zu sendenden Zeichen
* @return 1 bei Erfolg, -1 im Fehlerfall
*/
int gnublin_spi::send(unsigned char* tx, int length){
	int status;
	struct spi_ioc_transfer	xfer;
	xfer.tx_buf = (unsigned long)tx;
	xfer.len = length;
	xfer.rx_buf = 0;
	xfer.delay_usecs = 0;
	xfer.speed_hz = 0;
	xfer.bits_per_word = 0;
	status = ioctl(fd, SPI_IOC_MESSAGE(1), &xfer);
	if ( status < 0){
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return 1;
}


//****************************** message() ********************************
// send and read data over SPI bus (half duplex)
// paramters:	* [__u8*] tx	data which will be send
//		* [int] tx_length	length of data which will be send
//		* [__u8*] rx	buffer for data which will be recived
//		* [int] rx_length	length of data which will be recived
// return: 	* [int] 1	for success
//		* [int] -1  	for failure
/**
* @~english
* @brief Send and recive data over the SPI-Bus (half duplex)
*
* @param Data which will be send
* @param Length of data which will be send
* @param Buffer for recived datas
* @param length of recived datas
* @return 1 by success, -1 by failure
*
* @~german
* @brief Sendet und empfängt daten über den SPI-Bus (halb duplex).
*
* @param Zu sendende Daten
* @param Anzahl der zu sendenden Zeichen
* @param Buffer für den Datenempfang
* @param Anzahl der zu empfangenden Zeichen
* @return 1 bei Erfolg, -1 im Fehlerfall
*/
int gnublin_spi::message(unsigned char* tx, int tx_length, unsigned char* rx, int rx_length){
	int status;
	struct spi_ioc_transfer xfer[2];
	xfer[0].tx_buf = (unsigned long) tx;
	xfer[0].len = tx_length;
	xfer[0].rx_buf = 0;
	xfer[0].delay_usecs = 0;
	xfer[0].speed_hz = 0;
	xfer[0].bits_per_word = 0;
	
	xfer[1].rx_buf = (unsigned long) rx;
	xfer[1].len = rx_length;
	xfer[1].tx_buf = 0;
	xfer[1].delay_usecs = 0;
	xfer[1].speed_hz = 0;
	xfer[1].bits_per_word = 0;

	status = ioctl(fd, SPI_IOC_MESSAGE(2), xfer);
	if (status < 0){
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return 1;
}

