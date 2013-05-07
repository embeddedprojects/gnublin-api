#include "adc.h"

#if (BOARD == RASPBERRY_PI)
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
