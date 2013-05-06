#include "module_dogm.h"

//***************************************************************************
// Class for accesing the GNUBLIN MODULE-DISPLAY 2x16
//***************************************************************************


//********** Constructor ***************************
// set RS-PIN to 14 / for RPi to 4
// set RS-PIN as OUTPUT
gnublin_module_dogm::gnublin_module_dogm(){
#ifdef BOARD_RASPBERRYPI
	rs_pin=4;
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
