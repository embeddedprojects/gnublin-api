#include "module_adc.h"


//Hinweis: evtl. Fehlerhaft nicht getestet
//*****************************************************************************
// Class for accesing GNUBLIN Module-ADC / ADS7830
//*****************************************************************************

gnublin_module_adc::gnublin_module_adc() {
	i2c.setAddress(0x48);
	referenceValue = 2.5;
	reference_flag = 1;
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
		referenceValue = 3.3;
		reference_flag = 0;
	}
	else if (value == 1) {
		referenceValue = 2.5;
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
	return atoi((const char*)value);	
}


int gnublin_module_adc::getValue(int channel1, int channel2) {
	int command;
	unsigned char value[1];
	
	for (int i = 1; i<9; i=i+2) {
		if (channel1 == i && channel2 == (i+1)){
			switch(i) {
				case 1: command = 0x00; break;
				case 3: command = 0x10; break;
				case 5: command = 0x20; break;
				case 7: command = 0x30; break;
			}
		}
		else if (channel2 == i && channel1 == (i+1)) {
			switch(i) {
				case 1: command = 0x40; break;
				case 3: command = 0x50; break;
				case 5: command = 0x60; break;
				case 7: command = 0x70; break;
			}
		}
		else {
			error_flag = true;
			return -1;
		}
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
	return atoi((const char*)value);
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
