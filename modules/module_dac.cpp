#include "module_lm75.h"
//*******************************************************************
//Class for accessing the LM75 IC via I2C
//*******************************************************************

//------------------Konstruktor------------------
/** @~english 
* @brief Sets the error_flag to "false", the closemode to "1" (see i2c for details) and the standard i2c Address to 0x4f
*
* @~german 
* @brief Setzt das error_flag auf "false", den closemode auf "1" (siehe i2c für Details) und die Standard i2c Adresse auf 0x4f
*
*/
gnublin_module_dac::gnublin_module_dac()
{
	setAddress(0x60);
	_channel[0] = _channel[1] = _channel[2] = _channel[3] = 0;
}


//-------------get Error Message-------------
/** @~english 
* @brief Get the last Error Message.
*
* This function returns the last Error Message, which occurred in that Class.
* @return ErrorMessage as c-string
*
* @~german 
* @brief Gibt die letzte Error Nachricht zurück.
*
* Diese Funktion gibt die Letzte Error Nachricht zurück, welche in dieser Klasse gespeichert wurde.
* @return ErrorMessage als c-string
*/
/*
const char *gnublin_module_lm75::getErrorMessage(){
	return ErrorMessage.c_str();
}
*/


//-------------------------------Fail-------------------------------
/** @~english 
* @brief returns the error flag to check if the last operation went wrong
*
* @return error_flag as boolean
*
* @~german 
* @brief Gibt das error_flag zurück um zu überprüfen ob die vorangegangene Operation einen Fehler auweist
*
* @return error_flag als bool
*/
/*
bool gnublin_module_lm75::fail(){
	return error_flag;
}
*/



//-------------set Address-------------
/** @~english 
* @brief Set the i2c slave address 
*
* With this function you can set the individual I2C Slave-Address.
* @param Address new I2C slave Address
*
* @~german 
* @brief Setzt die i2c slave Adresse
*
* Mit dieser Funktion kann die individuelle I2C Slave-Adresse gesetzt werden.
* @param Address neue I2C slave Adresse
*/
void gnublin_module_dac::setAddress(int Address){
	i2c.setAddress(Address);
}


void gnublin_module_dac::write(int channel, int value) {
	_channel[channel] = value;   
	int i;
	char lowByte[4];
	char highByte[4];

	for (i=0; i<4; i++) {
		lowByte[i] = (char) (_channel[i] & 0xff);
		highByte[i] = (char) ((_channel[i] >> 8) & 0xff);
	}

	unsigned char buffer[8]={	highByte[0],lowByte[0],
														highByte[1],lowByte[1],
														highByte[2],lowByte[2],
														highByte[3],lowByte[3]};

   i2c.send(buffer,8);

}
