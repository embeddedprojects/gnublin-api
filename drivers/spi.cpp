#include "spi.h"


//***************************************************************************
// Class for accessing the SPI-Bus
//***************************************************************************


//********************* constructor *****************************************
// set standart devicefile "/dev/spidev0.11" with srandart chipselect pin 11
// opens devicefile

gnublin_spi::gnublin_spi(){
	error_flag = false;
	std::string device = "/dev/spidev0.11";
	fd = open(device.c_str(), O_RDWR);
	if (fd < 0) {
		system("modprobe spidev cs_pin=11");
		sleep(1);
		fd = open(device.c_str(), O_RDWR);
	}
}


//******************** destructor *******************************************
// close filehadler

gnublin_spi::~gnublin_spi(){
	close(fd);
}


//******************** fail() ***********************************************
// return: 	[bool] errorflag

bool gnublin_spi::fail(){
	return error_flag;
}


//-------------get Error Message-------------
// get the last ErrorMessage
// parameters:		NONE
// return:		[const char*]ErrorMessage	Error Message as c-string

const char *gnublin_spi::getErrorMessage(){
	return ErrorMessage.c_str();
}


//*********************** setCS *********************************************
// set custom chipselect and open filehandler
// paramters: 	[int] cs	Pin for chipselect
// return: 	* [int] 1	for success
//		* [int] -1  	for failure

int gnublin_spi::setCS(int cs){
	std::string cs_str = numberToString(cs);
	std::string device = "/dev/spidev0." + cs_str;
	fd = open(device.c_str(), O_RDWR);
	if (fd < 0) {
		std::string command = "modprobe spidev cs_pin=" + cs_str;
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
// set SPI Mode
// parameters:	[int] mode	Modenumber
// return: 	* [int] 1	for success
//		* [int] -1  	for failure

int gnublin_spi::setMode(unsigned char mode){
	if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0){
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return 1;
}


//***************** getMode *************************************************
// read Modenumber from device
// paramters:	NONE
// return:	[int] mode	Modenumber

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
// specific how the data will be send
// paramters: 	[int] lsb	0: MSB first, 1 LSB first
// return: 	* [int] 1	for success
//		* [int] -1  	for failure

int gnublin_spi::setLSB(unsigned char lsb){
	if (ioctl(fd, SPI_IOC_WR_LSB_FIRST, &lsb) < 0){
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return 1;
}


//************************ getLSB() *****************************************
// get LSB-Mode of the device
// paramters: NONE
// return:	[int] lsb	Modenumber of the LSB

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
// set Lenght of Words which will be send
// paramters: 	[__u8] bits	number of bits
// return: 	* [int] 1	for success
//		* [int] -1  	for failure

int gnublin_spi::setLength(unsigned char bits){
	if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0){
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return 1;
}


//************************ getLength() **************************************
// get word length of the device
// paramters:	NONE
// return:	[int] bits	number of bits

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
// set the SPI-bus speed in Hz
// paramters:	[__u32] speed	Speed in Hz
// return: 	* [int] 1	for success
//		* [int] -1  	for failure

int gnublin_spi::setSpeed(unsigned int speed){
	if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0){
		error_flag = true;
		return -1;
	}
	error_flag = true;
	return 1;
}


//************************* getSpeed() **************************************
// get speed from bus
// paramters	NONE
// return:	[int] speed	SPI Speed in Hz

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
// receive data from SPI Bus
// paramters:	* [char*] buffer	buffer for recived datas
//		* [int] len	length of data which will be recived
// return: 	* [int] 1	for success
//		* [int] -1  	for failure

int gnublin_spi::receive(char* buffer, int len){
	if (read(fd, buffer, len) < 0) {
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return 1;
}

//*************************** send() ****************************************
// send data over SPI bus
// paramters:	* [__u8*] tx	data which will be send
//		* [int] length	length of data which will be send
// return: 	* [int] 1	for success
//		* [int] -1  	for failure

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

