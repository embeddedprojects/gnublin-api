//********************************************
//GNUBLIN API -- MAIN FILE
//build date: 05/07/13 09:54
//******************************************** 

#include"gnublin.h"


//Converting string to number
int stringToNumber(std::string str){
	std::stringstream  strin;
	int var;

	strin << str;
	strin >> var;
	
	return var;
}

//Converting a number to string
std::string numberToString(int num){
	std::ostringstream strout;
	std::string str;

	strout << num;
	str = strout.str();

	return str;
}

//Converting string wich repesenting a hexnumber to number
int hexstringToNumber(std::string str){
	std::stringstream  strin;
	int var;

	strin << std::hex << str;
	strin >> var;
	
	return var;
}

gnublin_gpio::gnublin_gpio(){
	error_flag = false;
}


int gnublin_gpio::unexport(int pin){
	std::string pin_str = numberToString(pin);
	std::string dir = "/sys/class/gpio/unexport";
	std::ofstream file (dir.c_str());
	if (file < 0) {
		error_flag = true;
		return -1;
	}
	file << pin_str;
	file.close();
	error_flag = false;
	return 1;
}

bool gnublin_gpio::fail(){
	return error_flag;
}

//-------------get Error Message-------------
// get the last ErrorMessage
// parameters:		NONE
// return:		[const char*]ErrorMessage	Error Message as c-string

const char *gnublin_gpio::getErrorMessage(){
	return ErrorMessage.c_str();
}

int gnublin_gpio::pinMode(int pin, std::string direction){
	#ifndef BOARD_RASPBERRYPI
	if (pin == 4 && direction == "out"){
		error_flag = true;
		return -1;
	}
	#endif
	std::string pin_str = numberToString(pin);
	std::string dir = "/sys/class/gpio/export";
	std::ofstream file (dir.c_str());
	if (file < 0) {
		error_flag = true;
		return -1;
	}
	file << pin;
	file.close();

	dir = "/sys/class/gpio/gpio" + pin_str + "/direction";

	file.open(dir.c_str());
	if (file < 0) {
		error_flag = true;
		return -1;
	}
	file << direction;
	file.close();
	error_flag = false;
	return 1;
}

int gnublin_gpio::digitalWrite(int pin, int value){
	if (pin == 4){
		error_flag = true;
		return -1;
	}
	if (value != 0 && value != 1){
		error_flag = true;
		return -1;
	}
	std::string value_str = numberToString(value);
	std::string pin_str = numberToString(pin);
	std::string dir = "/sys/class/gpio/gpio" + pin_str + "/value";

	std::ofstream file (dir.c_str());
	if (file < 0) {
		error_flag = true;
		return -1;
	}
	file << value_str;
	file.close();
	error_flag = false;
	return 1;
}

int gnublin_gpio::digitalRead(int pin) {
	std::string value;
	
	std::string pin_str = numberToString(pin);
	std::string device = "/sys/class/gpio/gpio" + pin_str + "/value";
	std::ifstream file(device.c_str());
	if (file < 0){
		error_flag = true;
		return -1;
	}
	file >> value;
	file.close();
	error_flag = false;
	return stringToNumber(value);
	
}


//*******************************************************************
//Class for accessing GNUBLIN i2c Bus
//*******************************************************************

//------------------Konstruktor------------------
// set error flag false and default i2c dev file

gnublin_i2c::gnublin_i2c()
{
	devicefile="/dev/i2c-1";
	error_flag=false;
}

//-------------------------------Fail-------------------------------
//returns the error flag. if something went wrong, the flag is true

bool gnublin_i2c::fail(){
	return error_flag;
}

//-------------set Address-------------
// set the slave address
// parameters:		[int]Address	i2c slave Address
// return:		NONE

void gnublin_i2c::setAddress(int Address){
	slave_address = Address;
}

//-------------get Address-------------
// get the slave address
// parameters:		NONE
// return:		[int]Address	i2c slave Address

int gnublin_i2c::getAddress(){
	return slave_address;
}

//-------------get Error Message-------------
// get the last ErrorMessage
// parameters:		NONE
// return:		[const char*]ErrorMessage	Error Message as c-string

const char *gnublin_i2c::getErrorMessage(){
	return ErrorMessage.c_str();
}

//-------------------set devicefile----------------
// set the i2c device file. default is "/dev/i2c-1"
// parameters:		[string]filename	path to the dev file
// return:		NONE

void gnublin_i2c::setDevicefile(std::string filename){
	devicefile = filename;
}


//----------------------------------receive----------------------------------
// read from the i2c Bus			
// parameters:		[unsigned char]RegisterAddress 	specify the address of a register you want to read from
//			[char*]RxBuf 			receive buffer. The read bytes will be stored in it
// 			[int]length			amount of bytes that will be read
// return:		[int]  1			success
// 			[int] -1			failure
// 
// e.g.
// read 2 bytes into buf
// receive(buf, 2);
// 
// read 3 bytes into buf from a register with the address 0x12
// receive(0x12, buf, 3);
//---------------------------------------------------------------------------

int gnublin_i2c::receive(unsigned char *RxBuf, int length){
	error_flag=false;
	int fd;


	if ((fd = open(devicefile.c_str(), O_RDWR)) < 0) {
		ErrorMessage="ERROR opening: " + devicefile + "\n";
		error_flag=true;
		close(fd);    	
		return -1;
	}

	if (ioctl(fd, I2C_SLAVE, slave_address) < 0) {
		ErrorMessage="ERROR address: " + numberToString(slave_address) + "\n";
		error_flag=true; 
		close(fd);
    	return -1;
  	}

	if (read(fd, RxBuf, length) != length){
		ErrorMessage="i2c read error! Address: " + numberToString(slave_address) + " dev file: " + devicefile + "\n";		
		error_flag=true; 
		close(fd);
		return -1;
	}

	close(fd); 
	return 1;
}

int gnublin_i2c::receive(unsigned char RegisterAddress, unsigned char *RxBuf, int length){
	error_flag=false;	
	int fd;

	if ((fd = open(devicefile.c_str(), O_RDWR)) < 0) {
		ErrorMessage="ERROR opening: " + devicefile + "\n";
		error_flag=true; 
		close(fd);
    	return -1;
	}

	if (ioctl(fd, I2C_SLAVE, slave_address) < 0) {
		ErrorMessage="ERROR address: " + numberToString(slave_address) + "\n";
		error_flag=true; 
		close(fd);
    	return -1;
  	}

	if (write(fd, &RegisterAddress, 1) != 1){
		ErrorMessage="i2c write error!\n";
		error_flag=true; 
		close(fd);
		return -1;
		}


	if (read(fd, RxBuf, length) != length){
		ErrorMessage="i2c read error! Address: " + numberToString(slave_address) + " dev file: " + devicefile + "\n";
		error_flag=true; 
		close(fd);
		return -1;
	}

	close(fd); 	 
	return 1;
}


//-----------------------------------send-----------------------------------
// send data to the i2c Bus
//
// parameters:		[unsigned char]RegisterAddress 	specify the address of a register you want to write to
//			[char*]TxBuf 			transmit buffer. the data you want to send is stored in it
// 			[int]length			amount of bytes that will be sent.
// return:		[int]  1			success
// 			[int] -1			failure
//
//
// e.g.
// send 2 bytes from buf
// send (buf, 2);
// 
// send 3 bytes from buf to a register with the address 0x12
// send (0x12, buf, 3);
//---------------------------------------------------------------------------
int gnublin_i2c::send(unsigned char *TxBuf, int length){
	error_flag=false;	
	int fd; 

	if ((fd = open(devicefile.c_str(), O_RDWR)) < 0) {
		ErrorMessage="ERROR opening: " + devicefile + "\n";
		error_flag=true; 
		close(fd);
    	return -1;
	}

	if (ioctl(fd, I2C_SLAVE, slave_address) < 0) {
		ErrorMessage="ERROR address: " + numberToString(slave_address) + "\n";
		error_flag=true; 
		close(fd);
    	return -1;
  	}

	if(write(fd, TxBuf, length) != length){
		ErrorMessage="i2c write error!\n";
		error_flag=true; 
		close(fd);
		return -1;
	}

	close(fd);	
	return 1;
}

int gnublin_i2c::send(unsigned char RegisterAddress, unsigned char *TxBuf, int length){
	error_flag=false;	
	int fd, i;
	unsigned char data[length+1];
	data[0]=RegisterAddress;

	for ( i = 0; i < length ; i++ ) {
		data[ i + 1 ] = (char)TxBuf[ i ];
	}

	if ((fd = open(devicefile.c_str(), O_RDWR)) < 0) {
		ErrorMessage="ERROR opening: " + devicefile + "\n";
		error_flag=true; 
		close(fd);
    	return -1;
	}

	if (ioctl(fd, I2C_SLAVE, slave_address) < 0) {
		ErrorMessage="ERROR address: " + numberToString(slave_address) + "\n";
		error_flag=true; 
		close(fd);
    	return -1;
  	}
	
	
	if(write(fd, data, length+1) != length+1){
		ErrorMessage="i2c write error!\n";
		error_flag=true; 
		close(fd);
		return -1;
	}

	close(fd);	
	return 1;
}

int gnublin_i2c::send(int value){
	error_flag=false;
	int buffer[1];
	buffer[0]=value;	
	int fd; 

	if ((fd = open(devicefile.c_str(), O_RDWR)) < 0) {
		ErrorMessage="ERROR opening: " + devicefile + "\n";
		error_flag=true; 
		close(fd);
    	return -1;
	}

	if (ioctl(fd, I2C_SLAVE, slave_address) < 0) {
		ErrorMessage="ERROR address: " + numberToString(slave_address) + "\n";
		error_flag=true; 
		close(fd);
    	return -1;
  	}

	if(write(fd, buffer, 1) != 1){
		ErrorMessage="i2c write error!\n";
		error_flag=true; 
		close(fd);
		return -1;
	}

	close(fd);	
	return 1;
}


//***************************************************************************
// Class for accessing the SPI-Bus
//***************************************************************************


//********************* constructor *****************************************
// set standart devicefile "/dev/spidev0.11" with srandart chipselect pin 11
// opens devicefile

gnublin_spi::gnublin_spi(){
	error_flag = false;
	#ifdef BOARD_RASPBERRYPI
	std::string device = "/dev/spidev0.0";
	#else
	std::string device = "/dev/spidev0.11";
	#endif
	fd = open(device.c_str(), O_RDWR);
	if (fd < 0) {
		#ifdef BOARD_RASPBERRYPI
		system("modprobe spi-bcm2708");
		#else
		system("modprobe spidev cs_pin=11");
		#endif
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
		#ifdef BOARD_RASPBERRYPI
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


#ifndef BOARD_RASPBERRYPI
//****************************************************************************
// Class for easy acces to the GPAs
//****************************************************************************

gnublin_adc::gnublin_adc(){
	std::ifstream file("/dev/lpc313x_adc");
	if (file.fail()) {
		system("modprobe lpc313x_adc");
		sleep(1);
	}
	file.close();
	error_flag = false;
}

//-------------get Error Message-------------
// get the last ErrorMessage
// parameters:		NONE
// return:		[const char*]ErrorMessage	Error Message as c-string

const char *gnublin_adc::getErrorMessage(){
	return ErrorMessage.c_str();
}

int gnublin_adc::getValue(int pin){
	std::string value;
	
	std::string pin_str = numberToString(pin);
	std::string device = "/dev/lpc313x_adc";
	std::ofstream file(device.c_str());
	if (file < 0) {
		error_flag = true;
		return -1;
	}
	file << pin_str;
	file.close();
	std::ifstream dev_file(device.c_str());
	dev_file >> value;
	dev_file.close();
	error_flag = false;
	return hexstringToNumber(value);
}

int gnublin_adc::getVoltage(int pin){
	int value = getValue(pin);
	value = value*825/256;
	return value;
}

int gnublin_adc::setReference(int ref){
	error_flag = false;
	return 1;
}

bool gnublin_adc::fail(){
	return error_flag;
}

#endif

//***************************************************************************
// Class for accesing the GNUBLIN MODULE-DISPLAY 2x16
//***************************************************************************


//********** Constructor ***************************
// set RS-PIN to 14
// set RS-PIN as OUTPUT
gnublin_module_dogm::gnublin_module_dogm(){
#ifdef BOARD_RASPBERRYPI
	rs_pin = 4:
#else
	rs_pin = 14;
#endif
	gpio.pinMode(rs_pin, OUTPUT);
	init_flag = false;
	error_flag = false;	
}

//********* init()**********************************
// execute initialisation of the display
// parameters: 	NONE
// return: 	* [int] 1	for success
//		* [int] -1  	for failure


int gnublin_module_dogm::init(){
	__u8 init_str[] = {0x39, 0x14, 0x55, 0x6D, 0x78, 0x38, 0x0C, 0x01, 0x06};
	if (gpio.digitalWrite(rs_pin, LOW) <0){
			error_flag = true;
			return -1;
	}
	spi.setSpeed(100000);
	if (spi.send(init_str, 9) < 0){
		error_flag = true;
		return -1;
	}
	init_flag = true;
	error_flag = false;
	return 1;
}


//************ fail() *******************************
// paramters: NONE
// return: [bool]error_flag
bool gnublin_module_dogm::fail(){
	return error_flag;
}


//-------------get Error Message-------------
// get the last ErrorMessage
// parameters:		NONE
// return:		[const char*]ErrorMessage	Error Message as c-string

const char *gnublin_module_dogm::getErrorMessage(){
	return ErrorMessage.c_str();
}


//*********** setRsPin() *****************************
// set the custom RS Pin (signal for command or value) of the display 
// paramters:	[int]pin 	number of the RS pin
// return: 	[int]1 		for success

int gnublin_module_dogm::setRsPin(int pin){
	rs_pin = pin;
	gpio.pinMode(rs_pin, OUTPUT);
	return 1;
}


//*********** setCS ************************************
// set custom Chipselect (CS) of the SPI bus
// paramters:	[int]cs 	number of the chipselect line
// return: 	* [int] 1	for success
//		* [int] -1  	for failure

int gnublin_module_dogm::setCS(int cs){
	if (spi.setCS(cs) < 0){
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return 1;
}


//************ print() **********************************
// writes string on display
// paramters:	[char*] buffer	char array for writing on the display
// return: 	* [int] 1	for success
//		* [int] -1  	for failure

int gnublin_module_dogm::print(char* buffer){
	__u8 tmp[32];
	int len = strlen(buffer);
	if(!init_flag){
		init();
	}
	for(int i=0; i<len; i++){
		tmp[i] = buffer[i];
	}
	gpio.digitalWrite(rs_pin, HIGH);
	if (spi.send(tmp, len) < 0){
		gpio.digitalWrite(rs_pin, LOW);
		error_flag = true;
		return -1;
	}
	gpio.digitalWrite(rs_pin, LOW);
	error_flag = false;
	return 1;
}


//************ print() **********************************
// writes string on a specific line of the display
// paramters:	* [char*] buffer char array for writing on the display
//		* [int] line	number of the line (1,2) on which the output is writen
// return: 	* [int] 1	for success
//		* [int] -1  	for failure

int gnublin_module_dogm::print(char* buffer, int line){
	error_flag = false;
	if(!init_flag){
		init();
	}
	if (returnHome() < 0){
		return -1;
	}
	if (line == 1){
		print(buffer);
	}
	else if (line == 2) {
		offset(16);
		print(buffer);
	}
	else {
		error_flag = true;
		return -1;
	}
	return -1;
}


//************ print() **********************************
// writes string on a specific line of the display with offset
// paramters:	* [char*] buffer char array for writing on the display
//		* [int] line	number of the line (1,2) on which the output is writen
//		* [int] off	offset in the specific line (1-15)
// return: 	* [int] 1	for success
//		* [int] -1  	for failure

int gnublin_module_dogm::print(char* buffer, int line, int off){
	error_flag = false;
	if(!init_flag){
		init();
	}
	returnHome();
	if (line == 1){
		offset(off);
		print(buffer);
	}
	else if (line == 2) {
		off +=16;
		offset(off);
		print(buffer);
	}
	else {
		error_flag = true;
		return -1;
	}
	return -1;
}


//****************** offset() **********************************
// specific the position of the cursor
// paramters	[int] num	specifics the number of element which the cursor is set
// return: 	* [int] 1	for success
//		* [int] -1  	for failure

int gnublin_module_dogm::offset(int num){
	__u8 tmp;
	if(!init_flag){
		char init_str[2] = " ";
		init();
		print(init_str);
	}
	if (num >= 0 && num < 16){
		tmp = num + 128;
	}
	else if (num >= 16 && num < 32){
		tmp = num - 16 + 192;
	}
	else {
		error_flag = true;
		return -1;
	}
	if (spi.send(&tmp, 1) < 0){
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return 1;
}


//*************** clear() ****************************************
// clear the display, all chars are erased, no influence on the shift
// paramters: 	NONE
// return: 	* [int] 1	for success
//		* [int] -1  	for failure

int gnublin_module_dogm::clear(){
	__u8 clear_cmd = 0x01;
	if(!init_flag){
		init();
	}
	if (spi.send(&clear_cmd, 1) < 0){
		error_flag = true;
		return -1;
	}
	return 1;
}


//**************** returnHome() ************************************
// reset all display paramters, shift is errased
// paramters: 	NONE
// return: 	* [int] 1	for success
//		* [int] -1  	for failure

int gnublin_module_dogm::returnHome(){
	__u8 return_cmd = 0x02;
	if(!init_flag){
		init();
	}
	if (spi.send(&return_cmd, 1) < 0){
		error_flag = true;
		return -1;
	}
	return 1;
}	


//**************** shift() *****************************************
// shifts the display by a specific number
// Paramters:	[int] num	specifics the number of shifts, positiv: right shift, negativ: left shift
// return: 	* [int] 1	for success
//		* [int] -1  	for failure

int gnublin_module_dogm::shift(int num){
	__u8 shift_cmd;
	if(!init_flag){
		init();
	}
	if (num > 0){
		shift_cmd = 0x1C;
		for (int i=0; i < num; i++){
			if(spi.send(&shift_cmd, 1) < 0){
				error_flag = true;
				return -1;
			}
		}
	}
	else if (num < 0){
		shift_cmd = 0x18;
		for (int i=0; i > num; i--){
			if(spi.send(&shift_cmd, 1) < 0){
				error_flag = true;
				return -1;
			}
		}
	}
	else {
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return 1;
}


//*********************** controlDisplay() **********************************
// power on/off of the display, on/off cursor, on/off blinking
// paramters:	*[int] power	power on(1), off(0)
//		*[int] cursor	cursor on(1), off(0)
//		*[int] blink	blinking on(1), off(0)

int gnublin_module_dogm::controlDisplay(int power, int cursor, int blink) {
	__u8 display_cmd = 0x08;
	if(!init_flag){
		init();
	}
	if (power == 1) {
		display_cmd += 0x04;
	}
	if (cursor == 1) {
		display_cmd += 0x02;
	}
	if (blink == 1) {
		display_cmd += 0x01;
	}
	if (spi.send(&display_cmd, 1) < 0){
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return 1;
}

//-------------Konstruktor-------------
// set error flag=false

gnublin_module_lm75::gnublin_module_lm75()
{
	error_flag=false;
}


//-------------get Error Message-------------
// get the last ErrorMessage
// parameters:		NONE
// return:		[const char*]ErrorMessage	Error Message as c-string

const char *gnublin_module_lm75::getErrorMessage(){
	return ErrorMessage.c_str();
}

//-------------------------------Fail-------------------------------
//returns the error flag. if something went wrong, the flag is true
bool gnublin_module_lm75::fail(){
	return error_flag;
}

//-------------set Address-------------
// set the slave address
// parameters:		[int]Address	i2c slave Address
// return:			NONE

void gnublin_module_lm75::setAddress(int Address){
	i2c.setAddress(Address);
}


//-------------------set devicefile----------------
// set the i2c device file. default is "/dev/i2c-1"
// parameters:		[string]filename	path to the dev file
// return:			NONE

void gnublin_module_lm75::setDevicefile(std::string filename){
	i2c.setDevicefile(filename);
}


//-----------------------------------get Temp-----------------------------------
// reads the raw data via i2c from the LM75 chip and calculates the temperature
// parameters:		NONE
// return:			[int] temp		temperature in °C


int gnublin_module_lm75::getTemp(){
	short value=0;
	int temp;	
	unsigned char rx_buf[2];
	error_flag=false;

	if(i2c.receive(0x00, rx_buf, 2)>0){
		// rx_buf[0] = MSByte
		// rx_buf[1] = LSByte
		// save the MSB
		value = rx_buf[0];
		// make space for the LSB
		value<<=8;
		// save the LSB
		value |= rx_buf[1];
		// Bit 0-4 isn't used in the LM75, so shift right 5 times
		value>>=5;

		//check if temperature is negative
		if(rx_buf[0] & 0x80)
		{
			value = value|0xF800;
			value =~value +1;
			temp = value*(-0.125);
			return temp;
		}
		else { //temperature is positive
			temp = value*0.125;
			return temp;
		}


	}
	else{
		error_flag=true;
		ErrorMessage = "Error i2c receive\n";
		return 0;	
	}
}


//--------------------------------get Temp float--------------------------------
// reads the raw data via i2c from the LM75 chip and calculates the temperature
// parameters: 		NONE
// return:			[float] temp		temperature in float format: 12.345 °C


float gnublin_module_lm75::getTempFloat(){
	short value=0;
	float temp;	
	unsigned char rx_buf[2];
	error_flag=false;

	if(i2c.receive(0x00, rx_buf, 2)>0){
		// rx_buf[0] = MSByte
		// rx_buf[1] = LSByte
		// save the MSB
		value = rx_buf[0];
		// make space for the LSB
		value<<=8;
		// save the LSB
		value |= rx_buf[1];
		// Bit 0-4 isn't used in the LM75, so shift right 5 times
		value>>=5;

		//check if temperature is negative
		if(rx_buf[0] & 0x80)
		{
			value = value|0xF800;
			value =~value +1;
			temp = value*(-0.125);
			return temp;
		}
		else { //temperature is positive
			temp = value*0.125;
			return temp;
		}
		
	}
	else{	
		error_flag=true;
		ErrorMessage="Error i2c receive\n";
		return 0;
	}
		
}


//--------------------------------get Value---------------------------------
// reads the raw data via i2c from the LM75 chip and shift the bits correctly
// parameters:		NONE
// return:			[short]value		raw value, already shifted


short gnublin_module_lm75::getValue(){
	short value=0;
	unsigned char rx_buf[2];
	error_flag=false;
	if(i2c.receive(0x00, rx_buf, 2)>0){
		
		// rx_buf[0] = MSByte
		// rx_buf[1] = LSByte
		// save the MSB
		value = rx_buf[0];
		// make space for the LSB
		value<<=8;
		// save the LSB
		value |= rx_buf[1];
		// Bit 0-4 isn't used in the LM75, so shift right 5 times
		value>>=5;
		return value;
		
	}
	else{
		error_flag=true;
		ErrorMessage="Error i2c receive\n";
		return 0;
	}
}


//Hinweis: evtl. Fehlerhaft nicht getestet
//*****************************************************************************
// Class for accesing GNUBLIN Module-ADC / ADS7830
//*****************************************************************************

gnublin_module_adc::gnublin_module_adc() {
	i2c.setAddress(0x48);
	referenceValue = 2500;
	reference_flag = IN;
	error_flag = false;
}

//-------------get Error Message-------------
// get the last ErrorMessage
// parameters:		NONE
// return:		[const char*]ErrorMessage	Error Message as c-string

const char *gnublin_module_adc::getErrorMessage(){
	return ErrorMessage.c_str();
}


//------------ fail() -----------------------
// get error_flag
// parameters:		NONE
// return:		[bool] error_flag

bool gnublin_module_adc::fail() {
	return error_flag;
}


//-------------setAddress-------------
// set the slave address
// parameters:		[int]address	i2c slave Address
// return:		NONE

int gnublin_module_adc::setAddress(int address) {
	i2c.setAddress(address);
	if (i2c.fail()) {
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return 1;
}


//-------------------setDevicefile----------------
// set the i2c device file. default is "/dev/i2c-1"
// parameters:		[string]filename	path to the dev file
// return:		NONE

int gnublin_module_adc::setDevicefile(std::string filename) {
	i2c.setDevicefile(filename);
	if (i2c.fail()) {
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return 1;
}

//-------------------setReference() ----------------
// set reverence voltage to internal or external
// parameters:		[int] value	1 = internal (2.5V); 0 = external (3.3V)
// return:		NONE

int gnublin_module_adc::setReference(int value) {
	if (value == 0) {
		referenceValue = 3300;
		reference_flag = 0;
	}
	else if (value == 1) {
		referenceValue = 2500;
		reference_flag = 1;
	}
	else {
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return 1;
}


//---------------------- getValue() -----------------------
// get ADC Value of channel in reference to GND
// parameters:		[int] channel		ADC-Port
// return:		[int] value		value of ADC-channel

int gnublin_module_adc::getValue(int channel) {
	int command;
	unsigned char value[1];
	
	if (reference_flag == 0){
		switch (channel) {
			case 1: command = 0x87; break;
			case 2: command = 0xC7; break;
			case 3: command = 0x97; break;
			case 4: command = 0xD7; break;
			case 5: command = 0xA7; break;
			case 6: command = 0xE7; break;
			case 7: command = 0xB7; break;
			case 8: command = 0xF7; break;
			default: error_flag = true; return -1; break;
		}
	}
	else {
		switch (channel) {
			case 1: command = 0x8F; break;
			case 2: command = 0xCF; break;
			case 3: command = 0x9F; break;
			case 4: command = 0xDF; break;
			case 5: command = 0xAF; break;
			case 6: command = 0xEF; break;
			case 7: command = 0xBF; break;
			case 8: command = 0xFF; break;
			default: error_flag = true; return -1; break;
		}
	}
	
	i2c.send(command);
	if (i2c.fail()) {
		error_flag = true;
		return -1;
	}

	i2c.receive(value, 1);
	if (i2c.fail()) {
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return value[0];	
}


int gnublin_module_adc::getValue(int channel1, int channel2) {
	int command = -1;
	unsigned char value[1];
	
	for (int i = 1; i<9; i += 2 ) {
		if (channel1 == i && channel2 == (i+1)){
			switch(i) {
				case 1: command = 0x00; break;
				case 3: command = 0x10; break;
				case 5: command = 0x20; break;
				case 7: command = 0x30; break;
			}
		}
		if (channel2 == i && channel1 == (i+1)) {
			switch(i) {
				case 1: command = 0x40; break;
				case 3: command = 0x50; break;
				case 5: command = 0x60; break;
				case 7: command = 0x70; break;
			}
		}
	}
	if (command == -1){
		error_flag = true;
		return -1;
	}
	if (reference_flag == 0) {
		command += 0x7;
	}
	else {
		command += 0xF;
	}
	i2c.send(command);
	if (i2c.fail()) {
		error_flag = true;
		return -1;
	}

	i2c.receive(value, 1);
	if (i2c.fail()) {
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return value[0];
}


//---------------------- getVoltage() -----------------------
// get ADC Value of channel in reference to GND
// parameters:		[int] channel		ADC-Port
// return:		[int] voltage		voltage of ADC-channel in mV

int gnublin_module_adc::getVoltage(int channel) {
	error_flag = false;
	int voltage;
	int value = getValue(channel);
	if (error_flag) {
		return -1;
	}
	
	voltage = value * referenceValue / 255;
	return voltage;
}


int gnublin_module_adc::getVoltage(int channel1, int channel2) {
	error_flag = false;
	int voltage;
	int value = getValue(channel1, channel2);
	if (error_flag) {
		return -1;
	}
	
	voltage = value * referenceValue / 255;
	return voltage;
}

//*******************************************************************
//Class for accessing GNUBLIN Module-Portexpander or any PCA9555
//*******************************************************************

//-------------Konstruktor-------------
// set error flag=false

gnublin_module_pca9555::gnublin_module_pca9555() 
{
	error_flag=false;
}


//-------------get Error Message-------------
// get the last ErrorMessage
// parameters:		NONE
// return:			[const char*]ErrorMessage	Error Message as c-string

const char *gnublin_module_pca9555::getErrorMessage(){
	return ErrorMessage.c_str();
}

//-------------------------------Fail-------------------------------
//returns the error flag. if something went wrong, the flag is true
bool gnublin_module_pca9555::fail(){
	return error_flag;
}

//-------------set Address-------------
// set the slave address
// parameters:		[int]Address	i2c slave Address
// return:			NONE

void gnublin_module_pca9555::setAddress(int Address){
	i2c.setAddress(Address);
}


//-------------------set devicefile----------------
// set the i2c device file. default is "/dev/i2c-1"
// parameters:		[string]filename	path to the dev file
// return:			NONE

void gnublin_module_pca9555::setDevicefile(std::string filename){
	i2c.setDevicefile(filename);
}

//-------------------Pin Mode----------------
// Controls the Pin Mode.
// parameters:		[int]pin 		Number of the pin
//					[string]		direction OUTPUT/INPUT		
// returns:			[int]  1		success
// 					[int] -1		failure
int gnublin_module_pca9555::pinMode(int pin, std::string direction){
	error_flag=false;
	unsigned char TxBuf[1];
	unsigned char RxBuf[1];

	if (pin < 0 || pin > 15){
		error_flag=true;
		ErrorMessage="Pin Number is not between 0-15";
		return -1;
	}

	if(pin >= 0 && pin <= 7){ // Port 0

			TxBuf[0]=pow(2, pin); //convert pin into its binary form e. g. Pin 3 = 8

			if (i2c.receive(0x06, RxBuf, 1)>0){ //read the current state

				if (direction=="out"){
					TxBuf[0]=RxBuf[0] & ~TxBuf[0]; // at output you have to invert the pin you want to set und AND it to change only the pin 
					if(i2c.send(0x06, TxBuf, 1)>0){
					return 1;
					}
					else {
						error_flag=true;
						ErrorMessage="i2c.send Error";
						return -1;
					}
									
				}
				else if (direction=="in"){
					TxBuf[0]=RxBuf[0] | TxBuf[0]; // at input you just have to do a OR
					if(i2c.send(0x06, TxBuf, 1)>0){
					return 1;
					}
					else {
						error_flag=true;
						ErrorMessage="i2c.send Error";
						return -1;
					}
				}
				else{
					error_flag=true;
					ErrorMessage="direction != IN/OUTPUT";				
					return -1;			
				}
			}
			else {
			error_flag=true;
			ErrorMessage="i2c.receive Error";
			return -1;
			}
	}
	else if(pin >= 8 && pin <= 15){ // Port 1

			TxBuf[0]=pow(2, (pin-8)); //convert pin into its binary form e. g. Pin 3 = 8

			if(i2c.receive(0x07, RxBuf, 1)>0){ //read the current state
			
				if (direction=="out"){
					TxBuf[0]=RxBuf[0] & ~TxBuf[0];
					if(i2c.send(0x07, TxBuf, 1)>0){
					return 1;
					}
					else {
						error_flag=true;
						ErrorMessage="i2c.send Error";
						return -1;
					}					
				}
				else if (direction=="in"){
					TxBuf[0]=RxBuf[0] | TxBuf[0];
					if(i2c.send(0x07, TxBuf, 1)>0){
					return 1;
					}
					else {
						error_flag=true;
						ErrorMessage="i2c.send Error";
						return -1;
					}
				}
				else{
					error_flag=true;
					ErrorMessage="direction != IN/OUTPUT";				
					return -1;			
				}
			}
			else {
			error_flag=true;
			ErrorMessage="i2c.receive Error";
			return -1;
			}
	}
	else return -1;
}


//-------------------Port Mode----------------
// Controls the Port Mode.
// parameters:		[int]port 		Number of the pin
//					[string]		direction OUTPUT/INPUT		
// returns:			[int]  1		success
// 					[int] -1		failure

int gnublin_module_pca9555::portMode(int port, std::string direction){
	error_flag=false;
	unsigned char TxBuf[1];

	if (port < 0 || port > 1){
		error_flag=true;
		ErrorMessage="Port Number is not between 0-1";
		return -1;
	}

	if(port == 0){ // Port 0

			if (direction=="out"){
				TxBuf[0]=0x00;
				if(i2c.send(0x06, TxBuf, 1)>0){
				return 1;
				}
				else {
					error_flag=true;
					ErrorMessage="i2c.send Error";
					return -1;
				}
								
			}
			else if (direction=="in"){
				TxBuf[0]=0xff;
				if(i2c.send(0x06, TxBuf, 1)>0){
				return 1;
				}
				else {
					error_flag=true;
					ErrorMessage="i2c.send Error";
					return -1;
				}
			}
			else{
				error_flag=true;
				ErrorMessage="direction != IN/OUTPUT";				
				return -1;			
			}
	}
	else if(port==1){ // Port 1
		
			if (direction=="out"){
				TxBuf[0]=0x00;
				if(i2c.send(0x07, TxBuf, 1)>0){
				return 1;
				}
				else {
					error_flag=true;
					ErrorMessage="i2c.send Error";
					return -1;
				}					
			}
			else if (direction=="in"){
				TxBuf[0]=0xff;
				if(i2c.send(0x07, TxBuf, 1)>0){
				return 1;
				}
				else {
					error_flag=true;
					ErrorMessage="i2c.send Error";
					return -1;
				}
			}
			else{
				error_flag=true;
				ErrorMessage="direction != IN/OUTPUT";				
				return -1;			
			}
	}
	else return -1;
}

//-------------------digital write----------------
// writes a high or low to the Output
// parameters:		[int]pin 		Number of the pin
//					[int]value		High/Low		
// returns:			[int]  1		success
// 					[int] -1		failure

int gnublin_module_pca9555::digitalWrite(int pin, int value){
	error_flag=false;
	unsigned char TxBuf[1];
	unsigned char RxBuf[1];

	if (pin < 0 || pin > 15){
		error_flag=true;
		ErrorMessage="Pin Number is not between 0-15";
		return -1;
	}


	if(pin >= 0 && pin <= 7){ // Port 0

			TxBuf[0]=pow(2, pin); //convert pin into its binary form e. g. Pin 3 = 8

			if (i2c.receive(0x02, RxBuf, 1)>0){ //read the current state

				if (value==0){
					TxBuf[0]=(RxBuf[0] & ~TxBuf[0]); // at low you have to invert the pin you want to set and do a AND to change only the pin you want 
					if(i2c.send(0x02, TxBuf, 1)>0){
					return 1;
					}
					else {
						error_flag=true;
						ErrorMessage="i2c.send Error";
						return -1;
					}
									
				}
				else if (value==1){
					TxBuf[0]=(RxBuf[0] | TxBuf[0]); // at high you just have to do a OR
					if(i2c.send(0x02, TxBuf, 1)>0){
					return 1;
					}
					else {
						error_flag=true;
						ErrorMessage="i2c.send Error";
						return -1;
					}
				}
				else{
					error_flag=true;
					ErrorMessage="value != 1/0";				
					return -1;			
				}
			}
			else {
			error_flag=true;
			ErrorMessage="i2c.receive Error";
			return -1;
			}
	}
	else if(pin >= 8 && pin <= 15){ // Port 1

			TxBuf[0]=pow(2, (pin-8)); //convert pin into its binary form e. g. Pin 3 = 8

			if(i2c.receive(0x03, RxBuf, 1)>0){ //read the current state
				if (value==0){
					
					TxBuf[0]=RxBuf[0] & ~TxBuf[0];
					if(i2c.send(0x03, TxBuf, 1)>0){
					return 1;
					}
					else {
						error_flag=true;
						ErrorMessage="i2c.send Error";
						return -1;
					}					
				}
				else if (value==1){
					TxBuf[0]=RxBuf[0] | TxBuf[0];
					if(i2c.send(0x03, TxBuf, 1)>0){
					return 1;
					}
					else {
						error_flag=true;
						ErrorMessage="i2c.send Error";
						return -1;
					}
				}
				else{
					error_flag=true;
					ErrorMessage="value != HIGH/LOW";				
					return -1;			
				}
			}
			else {
			error_flag=true;
			ErrorMessage="i2c.receive Error";
			return -1;
			}
	}
	else return -1;
}

//-------------------write port----------------
// Writes one byte to a complete port
// parameters:	[int]port 0/1 		Number of the port
//				[unsigned char]value	Byte to write
//							
// returns:		[int]  1		succsess
// 				[int] -1		failure

int gnublin_module_pca9555::writePort(int port, unsigned char value){
	error_flag=false;
	unsigned char buffer[1];
	buffer[0]=value;

	if(port==0){ // Port 0
		if(i2c.send(0x02, buffer, 1)>0){
			return 1;
		}
		else {
			error_flag=true;
			ErrorMessage="i2c.send Error";
			return -1;
		}
	}
	else if(port==1){
		if(i2c.send(0x03, buffer, 1)>0){
			return 1;
		}
		else {
			error_flag=true;
			ErrorMessage="i2c.send Error";
			return -1;
		}
	}		
	else{
		error_flag=true;
		ErrorMessage="Pin Number is not between 0-1";
		return -1;		
	}
	
}

//-------------------digital read----------------
// reads the state of the inputs and returns it
// parameters:		[int]pin 		Number of the pin
//							
// returns:			[int]  0/1		logical level of the pin
// 					[int] -1		failure

int gnublin_module_pca9555::digitalRead(int pin) {
	error_flag=false;
	unsigned char RxBuf[1];

	if (pin < 0 || pin > 15){
		error_flag=true;
		ErrorMessage="Pin Number is not between 0-15\n";
		return -1;
	}
	
	if(pin >= 0 && pin <= 7){ // Port 0		
		if(i2c.receive(0x00, RxBuf, 1)>0){

				RxBuf[0]<<=(7-pin); // MSB is now the pin you want to read from
				RxBuf[0]&=128; // set all bits to 0 except the MSB
		
				if(RxBuf[0]==0){
					return 0;
				}
				else if (RxBuf[0]==128){
					return 1;
				}
				else{
					error_flag=true;
					ErrorMessage="bitshift failed\n";
					return -1;
				}
		}
		else{
			error_flag=true;
			ErrorMessage="i2c.receive Error";
			return -1;
		}
	}
	else if(pin >= 8 && pin <= 15){ // Port 1
		if(i2c.receive(0x01, RxBuf, 1)>0){

				RxBuf[0]<<=(15-pin); // MSB is now the pin you want to read from
				RxBuf[0]&=128;	// set all bits to 0 except the MSB	
		
				if(RxBuf[0]==0){
					return 0;
				}
				else if (RxBuf[0]==128){
					return 1;
				}
				else{
					error_flag=true;
					ErrorMessage="bitshift failed\n";
					return -1;
				}
		}
		else{
			error_flag=true;
			ErrorMessage="i2c.receive Error";
			return -1;
		}
	}
	error_flag=true;
	ErrorMessage="something went wrong";
	return -1;
}


//****************************************************************************
// Class for easy use of the GNUBLIN Module-Relay
//****************************************************************************


//------------------Konstruktor------------------
// set Port 0 to OUTPUT and LOW
// set Error Flag flase
// set standard i2c Address 0x20

gnublin_module_relay::gnublin_module_relay() {
	error_flag=false;
	pca9555.setAddress(0x20);
}


//-------------get Error Message-------------
// get the last ErrorMessage
// parameters:		NONE
// return:		[const char*]ErrorMessage	Error Message as c-string

const char *gnublin_module_relay::getErrorMessage(){
	return ErrorMessage.c_str();
}

//-------------------------------Fail-------------------------------
//returns the error flag. if something went wrong, the flag is true
bool gnublin_module_relay::fail(){
	return error_flag;
}

//-------------set Address-------------
// set the slave address
// parameters:		[int]Address	i2c slave Address
// return:			NONE

void gnublin_module_relay::setAddress(int Address){
	pca9555.setAddress(Address);
}


//-------------------set devicefile----------------
// set the i2c device file. default is "/dev/i2c-1"
// parameters:		[string]filename	path to the dev file
// return:			NONE

void gnublin_module_relay::setDevicefile(std::string filename){
	pca9555.setDevicefile(filename);
}

//-------------------switch Pin----------------
// switches 1 relay pin, error_flag is set on failure
// parameters:		[int]pin	Pin 1-8
//					[int]value	HIGH or LOW
// return:			[int]1		success
//					[int]-1		failure
int gnublin_module_relay::switchPin(int pin, int value) {
	error_flag=false;

	if (pin < 1 || pin > 8) {
		error_flag=true;
		ErrorMessage="pin is not between 1-8!\n";
		return -1;
	}
	if (pca9555.digitalWrite((pin-1), value) < 0) {
		error_flag=true;
		ErrorMessage="pca9555.digitalWrite failed! Address correct?\n";		
		return -1;
	}
	return 1;
}



//------------------Konstruktor------------------
// set irun and vmax to the default values 

gnublin_module_step::gnublin_module_step()
{
	irun = 15;
	vmax = 8;
}


//-------------get Error Message-------------
// get the last ErrorMessage
// parameters:		NONE
// return:		[const char*]ErrorMessage	Error Message as c-string

const char *gnublin_module_step::getErrorMessage(){
	return ErrorMessage.c_str();
}


//-------------set Address-------------
// set the slave address
// parameters:		[int]Address	i2c slave Address
// return:			NONE

void gnublin_module_step::setAddress(int Address){
	i2c.setAddress(Address);
}

//-------------------set devicefile----------------
// set the i2c device file. default is "/dev/i2c-1"
// parameters:		[string]filename	path to the dev file
// return:			NONE

void gnublin_module_step::setDevicefile(std::string filename){
	i2c.setDevicefile(filename);
}

//-------------------set irun----------------
// set the Irun = newIrun
// parameters:		[unsigned int]newIrun	new Irun Value
// returns:			[int]  1			success
// 					[int] -1			failure

int gnublin_module_step::setIrun(unsigned int newIrun){
	if(newIrun <= 15){
		irun=newIrun;
		return 1;
	}
	else return -1;
}

//-------------------set ihold----------------
// set the ihold = newIhold
// parameters:		[unsigned int]newIhold	new ihold Value
// returns:			[int]  1				success
// 					[int] -1				failure
int gnublin_module_step::setIhold(unsigned int newIhold){
	if(newIhold <= 15){
		ihold=newIhold;
		return 1;
	}
	else return -1;
}

//-------------------set vmax----------------
// set the vmax = newVmax
// parameters:		[unsigned int]newVmax	new vmax Value
// returns:			[int]  1				success
// 					[int] -1				failure
int gnublin_module_step::setVmax(unsigned int newVmax){
	if(newVmax <= 15){
		vmax=newVmax;
		return 1;
	}
	else return -1;
}

//-------------------set vmin----------------
// set the vmin = newVmin
// parameters:		[unsigned int]newVmin	new vmin Value
// returns:			[int]  1				success
// 					[int] -1				failure
int gnublin_module_step::setVmin(unsigned int newVmin){
	if(newVmin <= 15){
		vmin=newVmin;
		return 1;
	}
	else return -1;
}


//-------------------write TMC----------------
// write to TMC222 Chip
// parameters:	[int]num			amount of bytes
//				[char*]TxBuf 		transmit buffer. the data you want to send is stored in it	
// return:		[int]  1			success
// 				[int] -1			failure

int gnublin_module_step::writeTMC(unsigned char *TxBuf, int num){
	if(!i2c.send(TxBuf, num)){
	    return -1;
   	}
	else return 1;
}

//-------------------read TMC----------------
// read from TMC222 Chip
// parameters:	[int]num			amount of bytes
//				[char*]RxBuf		receive buffer. The Data will be stored in it. 
// return:		[int]  1			success
// 				[int] -1			failure

int gnublin_module_step::readTMC(unsigned char *RxBuf, int num){
   	if(!i2c.receive(RxBuf, num)){
       	return -1;
    }
	else return 1;	
}

//-------------------burn new Address----------------
// burn a new i2c slave address into the TMC222 Chip
// parameters:	[int]new_address	the new slave address, which will be burned into the TMC222 Chip
//
// return:		[int]  1			success
// 				[int] -1			failure

int gnublin_module_step::burnNewAddress(int new_address){
	
	//SetOTPParam
	unsigned char buffer[5];
	int new_ad = 0;
	int old_ad = 0;
	int slave_address=i2c.getAddress();
	std::string yes = "NO!";

 	if(new_address <= slave_address){
		printf("\tThe new address must be higher than the old one (0x%x)!\n",slave_address);
		return -1;
 	}
 	else if (new_address > 0x7f){
		printf("The biggest slave address of the TMC222 is 0x7f\n");
		return -1;
 	}
 	else{
  		old_ad = (slave_address & 0x1e) >> 1;
  		new_ad = (new_address & 0x1e) >> 1;
  		if(((new_ad & 0x1)<(old_ad & 0x1))|((new_ad & 0x2)<(old_ad & 0x2))|((new_ad & 0x4)<(old_ad & 0x4))|((new_ad & 0x8)<(old_ad & 0x8))){
        		printf("\tThis address could not be set, because the '1' cant be undone!\n"
        			"\told OTP AD: 0x%x\n"
        			"\tnew OTP AD: 0x%x\n",old_ad, new_ad);
        		return -1;
  		}
	  	if((new_address & 0x01) == 1){
			printf("\tThe LSB address bit is set by the jumper on the module-step\n");
			new_address --;
			printf("\tThe new address will be set to: 0x%x \n", new_address);
	  	}

	    	printf("\tIf a bit of the OTP (on time programmable) is set, it cant be undone! \n\t"
	    		"If you are sure to write the new Address (0x%x) then type 'yes' in CAPITALS\n\n\t", new_address);
	    	std::cin >> yes;
	    	if(yes == "YES"){
			buffer[0] = 0x90; //SetOTPParam
			buffer[1] = 0xff; //N/A
			buffer[2] = 0xff; //N/A
			buffer[3] = 0x02; //set AD3 AD2 AD1 AD0
			buffer[4] = (unsigned char) new_ad;

		   	if(!i2c.send(buffer, 5)){
			   	return -1;
			}
			else {
				printf("\tNew Address was successfully set to: 0x%x\n\tPlease replug the module.\n\n", new_address);
				return 1;
			}
	    	}
	  	else{
			printf("\tYou didn't type 'YES'\n");
			return -1;
	  	}
	}
}

//-------------------get Full Status 1----------------
// get full status 1 from the TMC222
// parameters:	NONE
//
// return:		[int]  1			success
// 				[int] -1			failure

int gnublin_module_step::getFullStatus1(){
      	if(i2c.send(0x81)){
		return 1;		
		}
		else return -1;
}

//-------------------get Full Status 2----------------
// get full status 2 from the TMC222
// parameters:	NONE
//
// return:		[int]  1			success
// 				[int] -1			failure


int gnublin_module_step::getFullStatus2(){
		if(i2c.send(0xfc)){
			return 1;
		}
		else return -1;
}


//-------------------run Init----------------
// send Run Init command to the TMC222
// parameters:	NONE
//
// return:		[int]  1			success
// 				[int] -1			failure


int gnublin_module_step::runInit(){
		if(i2c.send(0x88)){
		return 1;
		}
		else return -1;
}


//-------------------set Motor Parameter----------------
// set the Motor parameters
// parameters:	[unsigned int]newIrun		new irun value
//				[unsigned int]newIhold		new ihold value
//				[unsigned int]newVmax		new vmax value
//				[unsigned int]newVmin		new vmin value
//
// return:		[int]  1				success
// 				[int] -1				failure


int gnublin_module_step::setMotorParam(){
	unsigned char buffer[8];
	//SetMotorParam
	buffer[0] = 0x89; //SetMotorParam
	buffer[1] = 0xff; //N/A
	buffer[2] = 0xff; //N/A
	buffer[3] = (unsigned char) ((irun * 0x10) + ihold); //Irun & I hold
	buffer[4] = (unsigned char) ((vmax * 0x10) + vmin); //Vmax & Vmin 
	buffer[5] = 0x00; //status
	buffer[6] = 0x00; //securePos
	buffer[7] = 0x00; //StepMode

    if(i2c.send(buffer, 8)){
	return 1;
	}
	else return -1;
}

int gnublin_module_step::setMotorParam(unsigned int newIrun, unsigned int newIhold, unsigned int newVmax, unsigned int newVmin){
	irun=newIrun;
	ihold=newIhold;
	vmax=newVmax;
	vmin=newVmin;

	unsigned char buffer[8];
	//SetMotorParam
	buffer[0] = 0x89; //SetMotorParam
	buffer[1] = 0xff; //N/A
	buffer[2] = 0xff; //N/A
	buffer[3] = (unsigned char) ((irun * 0x10) + ihold); //Irun & I hold
	buffer[4] = (unsigned char) ((vmax * 0x10) + vmin); //Vmax & Vmin 
	buffer[5] = 0x00; //status
	buffer[6] = 0x00; //securePos
	buffer[7] = 0x00; //StepMode

    if(i2c.send(buffer, 8)){
	return 1;
	}
	else return -1;
}

//-------------------hard stop----------------
// send hard stop command to the TMC222
// parameters:	NONE
//
// return:		[int]  1			success
// 				[int] -1			failure

int gnublin_module_step::hardStop(){
		if(i2c.send(0x85)){
		return 1;
		}
		else return -1;
}


//-------------------soft stop----------------
// send soft stop command to the TMC222
// parameters:	NONE
//
// return:		[int]  1			success
// 				[int] -1			failure

int gnublin_module_step::softStop(){
		if(i2c.send(0x8f)){
		return 1;
		}
		else return -1;
}

//-------------------reset position----------------
// send reset Position command to the TMC222 and sets the postion to 0
// parameters:	NONE
//
// return:		[int]  1			success
// 				[int] -1			failure

int gnublin_module_step::resetPosition(){
		if(i2c.send(0x86)){
		return 1;
		}
		else return -1;
}

//-------------------set position----------------
// send set Position command to the TMC222 
// parameters:	[int]position		the new position
//
// return:		[int]  1			success
// 				[int] -1			failure

int gnublin_module_step::setPosition(int position){
	unsigned char buffer[5];
	buffer[0] = 0x8B;   // SetPosition Command
	buffer[1] = 0xff;   // not avialable
	buffer[2] = 0xff;   // not avialable
	buffer[3] = (unsigned char) (position >> 8);  // PositionByte1 (15:8)
	buffer[4] = (unsigned char)  position;       // PositionByte2 (7:0)
	
	if(i2c.send(buffer, 5)){
		return 1;
	}
	else return -1;
}

//-------------------drive----------------
// send set Position command to the TMC222 
// parameters:	[int]steps		the amount of steps to drive (< 0 in negative direction, > 0 in positiv direction)
//
// return:		[int]  1			success
// 			[int] -1			failure
//

int gnublin_module_step::drive(int steps){
	int old_position;
	int new_position;

	old_position = getActualPosition();
	new_position = old_position+steps;
	if(setPosition(new_position)){
		return 1;
	}
	else return -1;
}

//-------------------get Motion Status----------------
// Indicates the actual behavior of the position controller. 
// parameters:	NONE
//
//return:[int]
//	0: Actual Position = Target Position; Velocity= 0
//	1: Positive Acceleration; Velocity > 0
//	2: Negative Acceleration; Velocity > 0
//	3: Acceleration = 0 Velocity = maximum pos Velocity
//	4: Actual Position /= Target Position; Velocity= 0
//	5: Positive Acceleration; Velocity < 0
//	6: Positive Acceleration; Velocity < 0
//	7: Acceleration = 0 Velocity = maximum neg Velocity 
// 
//	-1			failure

int gnublin_module_step::getMotionStatus(){
	unsigned char RxBuf[8];
	int motionStatus = -1;
	getFullStatus1();
	
    	if(!i2c.receive(RxBuf, 8))
		return -1;
	motionStatus = (RxBuf[5] & 0xe0) >> 5;
	return motionStatus;
	
}


//-------------------get Switch status----------------
// check the Switch Status
// parameters:	NONE
//
// return:		[int]  1			Switch closed
//				[int]  0			Switch open
// 				[int] -1			failure

int gnublin_module_step::getSwitch(){
	unsigned char RxBuf[8];    	
	int swi = 0;

    	getFullStatus1();

    	if(i2c.receive(RxBuf, 8)){
	
			if(RxBuf[5] & 0x10){
				swi = 1;				
			}
			else{
				swi = 0;
			}
			return swi;
		}	
		else return -1;
}

//-------------------get actual position----------------
// gets the actual position
// parameters:	NONE
//
// return:		[int]actualPosition		the actual position
// 				[int] -1				failure

int gnublin_module_step::getActualPosition(){
	unsigned char RxBuf[8];	
	int actualPosition=-1;
	
	if(getFullStatus2()==-1)
		return -1;
	
	if(i2c.receive(RxBuf, 8)){
		actualPosition = (RxBuf[1]<<8 | RxBuf[2]);
	}	
	return actualPosition;
}


//*******************************************************************
//Class for accessing GNUBLIN Module-LCD 4x20
//*******************************************************************

//-------------Konstruktor-------------
// set error flag=false

gnublin_module_lcd::gnublin_module_lcd() 
{
	version = (char *) "0.3";
	error_flag=false;
}


/** @~english 
* @brief Get the last Error Message.
*
* This Funktion returns the last Error Message, which occurred in that Class.
* @return ErrorMessage as c-string
*
* @~german 
* @brief Gibt die letzte Error Nachricht zurück.
*
* Diese Funktion gibt die Letzte Error Nachricht zurück, welche in dieser Klasse gespeichert wurde.
* @return ErrorMessage als c-string
*/
const char *gnublin_module_lcd::getErrorMessage(){
	return ErrorMessage.c_str();
}


/** @~english 
* @brief Returns the error flag. 
*
* If something went wrong, the flag is true.
* @return bool error_flag
*
* @~german 
* @brief Gibt das Error Flag zurück.
*
* Falls das Error Flag in der Klasse gesetzt wurde, wird true zurück gegeben, anderenfalls false.
* @return bool error_flag
*/
bool gnublin_module_lcd::fail(){
	return error_flag;
}


/** @~english 
* @brief Set the slave address 
*
* With this function you can set the individual I2C Slave-Address of the module.
* @param Address new I2C slave Address
*
* @~german 
* @brief Gibt das Error Flag zurück.
*
* Mit dieser Funktion kann die individuelle I2C Slave-Adresse des Moduls gesetzt werden.
* @param Address neue I2C slave Adresse
*/
void gnublin_module_lcd::setAddress(int Address){
	pca.setAddress(Address);
}


/** @~english 
* @brief Set devicefile.
*
* With this function you can change the I2C device file. Default is "/dev/i2c-1"
* @param filename path to the I2C device file
*
* @~german 
* @brief Setzt Device Datei.
*
* Mit dieser Funktion kann die I2C Gerätedatei geändert werden. Standartmäßig wird "/dev/i2c-1" benutzt.
* @param filename Pfad zur I2C Gerätedatei
*/
void gnublin_module_lcd::setDevicefile(std::string filename){
	pca.setDevicefile(filename);
}

/** @~english 
* @brief LCD out.
*
* This Funktion sends the given data and RS/RW pins to the pca9555 chip.
* @param rsrw contains the RS/RW pins
* @param data contains the data to send
* @return success: 1, failure: -1
*
* @~german 
* @brief Sendet Daten
*
* Diese Funktion sendet die übergebenen Daten und die RS/RW pins an den pca9555.
* @param rsrw beinhalten die RS/RW pins
* @param data Daten die an das Display gesendet werden sollen
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_lcd::out(unsigned char rsrw, unsigned char data ){
	if(!pca.writePort(0, data)){			//send data on Port 0
		error_flag=true;
		ErrorMessage = pca.getErrorMessage();
		return -1;
	}
	if(!pca.writePort(1, rsrw)){			//send RS/RW bits on Port 1
		error_flag=true;
		ErrorMessage = pca.getErrorMessage();
		return -1;
	}
	if(!pca.writePort(1, (rsrw | LCD_EN))){	//enable on
		error_flag=true;
		ErrorMessage = pca.getErrorMessage();
		return -1;
	}
	usleep(LCD_ENABLE_US);
	if(!pca.writePort(1, rsrw)){			//enable off
		error_flag=true;
		ErrorMessage = pca.getErrorMessage();
		return -1;
	}
	return 1;
}

/** @~english 
* @brief Send Data to the LCD
*
* This Funktion sends the given char to the display.
* @param data contains the data to send
* @return success: 1, failure: -1
*
* @~german 
* @brief Sendet Daten an das Display.
*
* Diese Funktion sendet die übergebenen Daten an das Display.
* @param data Daten die an das Display gesendet werden sollen
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_lcd::sendData(unsigned char data){
        if(!out(LCD_RS, (unsigned char) data)){
			return -1;
		}
        usleep(LCD_WRITEDATA_US);
        return 1;
}

/** @~english 
* @brief Send command to the LCD
*
* This Funktion sends an command to the display.
* @param data the command to send
* @return success: 1, failure: -1
*
* @~german 
* @brief Sendet einen Befehl an das Display.
*
* Diese Funktion sendet die übergebenen Befehle an das Display.
* @param data Befehl die an das Display gesendet werden sollen
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_lcd::command(unsigned char data){
        if(!out(0x00, (unsigned char) data)){
		    return -1;
		}
        usleep(LCD_COMMAND_US);
        return 1;
}


/** @~english 
* @brief Clear the LCD.
*
* Everythin on the Display will be erased
* @return success: 1, failure: -1
*
* @~german 
* @brief Bereinigt das Display
*
* Alles was auf dem Display angezeigt wird, wird gelöscht.
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_lcd::clear(){
        if(!command(LCD_CLEAR_DISPLAY)){
			return -1;
		}
        usleep(LCD_CLEAR_DISPLAY_MS);
        return 1;
}


/** @~english 
* @brief Curse home command.
*
* Returns the Cursor to the first possition.
* @return success: 1, failure: -1
*
* @~german 
* @brief Setzt den Cursor auf Anfangsposition oben links  
*
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_lcd::home(){
        if(!command(LCD_CURSOR_HOME)){
			return -1;
		}
        usleep(LCD_CURSOR_HOME_MS);
        return 1;
}


/** @~english 
* @brief Set display command.
*
* @return success: 1, failure: -1
*
* @~german 
* @brief Display Einstellungen Befehl.
*
* Mit dieser Funktion können Cursor-Einstellungen am Display vorgenommen werden. Wenn die Übergabe cursor == 1 ist, wird der Cursor am Display an der momentanen Stelle angezeigt(_). Wird blink == 1 übergeben, blinkt ein viereckiger Cursor. 
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_lcd::setdisplay(int cursor, int blink){
        unsigned char set_display;
        //Display ON/OFF Control
        set_display = LCD_SET_DISPLAY + LCD_DISPLAY_ON;
        if(cursor) set_display = set_display + LCD_CURSOR_ON;
        if(blink) set_display = set_display + LCD_BLINKING_ON;
        if(!command(set_display))
        	return -1;
        return 1;
}


/** @~english 
* @brief Set Cursor Command.
*
* Sets the Cursor to the given Position on the Display.
* @param x The Line 
* @param y The Column 
* @return success: 1, failure: -1
*
* @~german 
* @brief Setzt den Cursor an die übergebene Position auf das Display. 
*
* @param x Die Zeile 
* @param y Die Spalte
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_lcd::setcursor(unsigned char x, unsigned char y){
        unsigned char data;
        switch(x){
                case 1: //1. Zeile
                        data = LCD_SET_DDADR + LCD_DDADR_LINE1 + y;
                        break;

                case 2:    // 2. Zeile
                        data = LCD_SET_DDADR + LCD_DDADR_LINE2 + y;
                        break;

                case 3:    // 3. Zeile
                        data = LCD_SET_DDADR + LCD_DDADR_LINE3 + y;
                        break;

                case 4:    // 4. Zeile
                        data = LCD_SET_DDADR + LCD_DDADR_LINE4 + y;
                         break;

                default:
                		error_flag=true;
						ErrorMessage = "Wrong line/column";
                        return -1;
        }
        if(!command(data)){
        	return -1;
        }
        return 1;
}


/** @~english 
* @brief Sends the string to the display.
*
* @param data string to send 
* @return success: 1, failure: -1
*
* @~german 
* @brief Sendet den String an das Display. 
*
* Dieser Funktion kann ein String übergeben werden, welcher auf dem Display angezeigt werden soll. 
* @param data String zum senden 
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_lcd::string(const char *data){
        while(*data != '\0'){
                if(!sendData( *data++)){
                	return -1;
                }
        }
        return 1;
}


/** @~english 
* @brief Initializes the LCD.
*
* @return success: 1, failure: -1
*
* @~german 
* @brief Initialisiert das LCD.
*
* Wenn diese Funktion aufgerufen wird, wird der pca9555 so eingestellt, um das Display ansteuern zu können. Außerdem werden einige Befehle an das Display gesendet, um es auf das Anzeigen von Text vorbereitet. 
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_lcd::init(){
	//Set Ports as output
	if(!pca.portMode(0, "out")){ 	//Port 0 as Output
		error_flag=true;
		ErrorMessage = pca.getErrorMessage();
		return -1;
	}
	if(!pca.portMode(1, "out")){ 	//Port 1 as Output
		error_flag=true;
		ErrorMessage = pca.getErrorMessage();
		return -1;
	}

	//// initial alle Ausgänge auf Null
	if(!pca.writePort(0, 0x00)){
		error_flag=true;
		ErrorMessage = pca.getErrorMessage();
		return -1;
	}
	if(!pca.writePort(1, 0x00)){
		error_flag=true;
		ErrorMessage = pca.getErrorMessage();
		return -1;
	}
	
	usleep(LCD_BOOTUP_MS);

	//function set
	if(!command(LCD_SET_FUNCTION |
                        LCD_FUNCTION_8BIT |
                        LCD_FUNCTION_2LINE |
                        LCD_FUNCTION_5X7)){
		return -1;
	}
	usleep(LCD_SET_8BITMODE_MS);

	//Display ON/OFF Control
	if(!setdisplay(0, 0)){
		return -1;
	}

	if(!clear()){
		return -1;
	}
	
	//entry mode set
	if(!command(LCD_SET_ENTRY |
						LCD_ENTRY_INCREASE |
						LCD_ENTRY_NOSHIFT)){
		return -1;
	}
	
	return 1;
}
