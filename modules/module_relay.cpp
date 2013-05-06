#include "module_relay.h""


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


