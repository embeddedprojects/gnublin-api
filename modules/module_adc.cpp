#include "module_adc.h"


//*****************************************************************************
// Class for accesing GNUBLIN Module-ADC / ADS7830
//*****************************************************************************

/**
* @~english
* @brief Set the default I2C Address and reference to intern
*
* Default I2C Address: 0x48
*
* @~german
* @brief Setzt die standard I2C Adresse und die Referenzspannung auf intern
*
* Standard I2C Addresse: 0x48
*/
gnublin_module_adc::gnublin_module_adc() {
	i2c.setAddress(0x48);
	referenceValue = 2500;
	reference_flag = IN;
	error_flag = false;
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
const char *gnublin_module_adc::getErrorMessage(){
	return ErrorMessage.c_str();
}


//------------ fail() -----------------------

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
bool gnublin_module_adc::fail() {
	return error_flag;
}


//-------------setAddress-------------
// set the slave address
// parameters:		[int]address	i2c slave Address
// return:		NONE
/**
* @~english
* @brief Set the I2C Address
*
* @param address I2C Address
* @return 1 by success, -1 by failure
*
* @~german
* @brief Setzt die I2C Adresse.
*
* @param address I2C Adresse
* @return 1 bei Erfolg, -1 im Fehlerfall
*/
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
/**
* @~english
* @brief Set a custom I2C devicefile
*
* default devicefile: "/dev/i2c-1"
* @param filename path of the devicefile
* @return 1 by success, -1 by failure
*
* @~german
* @brief Setzt ein benutzerdefiniertes Devicefile.
*
* Standard Devicefile ist "/dev/i2c-1"
* @param filename Pfad zum Devicefile
* @return 1 bei Erfolg, -1 im Fehlerfall
*/
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

/**
* @~english
* @brief Set the reverencevoltage to intern or extern
*
* @param value IN (1) for intern (2.5V), OUT (0) for extern (3.3V)
* @return 1 by success, -1 by failure
*
* @~german
* @brief Setzt die referenzspannung auf intern oder extern.
*
* @param value IN (1) für intern (2,5V), OUT (0) für extern (3,3V)
* @return 1 bei Erfolg, -1 im Fehlerfall
*/
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

/**
* @~english
* @brief Get a value of an ADC channel in reference to GND
*
* @param channel Number of the ADC-channel (1-8)
* @return value
*
* @~german
* @brief Liefert den Wert eine ADC Ports bezogen zu GND
*
* @param channel Nummer des ADC-Ports (1-8)
* @return Wert
*/
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

/**
* @~english
* @brief Get a value of an ADC channel in reference to a second ADC channel (differential)
*
* This combinations are possible<br>
* 1 - 2<br>
* 3 - 4<br>
* 5 - 6<br>
* 7 - 8<br>
* 2 - 1<br>
* 4 - 3<br>
* 6 - 5<br>
* 8 - 7<br>
* @param channel1 Number of the first ADC-channel 
* @param channel2 Number of the second ADC-channel
* @return value
*
* @~german
* @brief Liefert den Wert eines ADC Ports bezogen auf einen zweiten ADC Port (differentielle Messung)
*
* Dies Kombinationen sind möglich:<br>
* 1 - 2<br>
* 3 - 4<br>
* 5 - 6<br>
* 7 - 8<br>
* 2 - 1<br>
* 4 - 3<br>
* 6 - 5<br>
* 8 - 7<br>
* @param channel1 Nummer des ersten ADC-Ports
* @param channel2 Nummer des zweiten ADC-Ports
* @return Wert
*/
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
/**
* @~english
* @brief Get the voltage of an ADC channel in reference to GND in mV
*
* @param channel Number of the ADC-channel (1-8)
* @return value in mV
*
* @~german
* @brief Liefert den Wert eine ADC Ports bezogen zu GND in mV
*
* @param channel Nummer des ADC-Ports (1-8)
* @return Wert in mV
*/
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

/**
* @~english
* @brief Get the voltage of an ADC channel in reference to a second ADC channel in mV (differential)
*
* This combinations are possible<br>
* 1 - 2<br>
* 3 - 4<br>
* 5 - 6<br>
* 7 - 8<br>
* 2 - 1<br>
* 4 - 3<br>
* 6 - 5<br>
* 8 - 7<br>
* @param channel1 Number of the first ADC-channel 
* @param channel2 Number of the second ADC-channel
* @return voltage in mV
*
* @~german
* @brief Liefert den Wert eines ADC Ports bezogen auf einen zweiten ADC Port in mV (differentielle Messung)
*
* Dies Kombinationen sind möglich:<br>
* 1 - 2<br>
* 3 - 4<br>
* 5 - 6<br>
* 7 - 8<br>
* 2 - 1<br>
* 4 - 3<br>
* 6 - 5<br>
* 8 - 7<br>
* @param channel1 Nummer des ersten ADC-Ports
* @param channel2 Nummer des zweiten ADC-Ports
* @return Wert in mV
*/
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
