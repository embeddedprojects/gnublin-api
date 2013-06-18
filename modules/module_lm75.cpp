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
gnublin_module_lm75::gnublin_module_lm75()
{
	error_flag=false;
	setAddress(0x4f);
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
const char *gnublin_module_lm75::getErrorMessage(){
	return ErrorMessage.c_str();
}

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
bool gnublin_module_lm75::fail(){
	return error_flag;
}

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
void gnublin_module_lm75::setAddress(int Address){
	i2c.setAddress(Address);
}


//-------------------set devicefile----------------
/** @~english
* @brief set i2c the device file. default is "/dev/i2c-1"
*
* This function sets the devicefile you want to access. by default "/dev/i2c-1" is set.
* @param filename path to the devicefile e.g. "/dev/i2c-0"
*
* @~german
* @brief setzt die I2C Device Datei. Standard ist die "/dev/i2c-1"
*
* Diese Funktion setzt die Geräte Datei, auf die man zugreifen möchte. Standardmäßig ist bereits "/dev/i2c-1" gesetzt.
* @param filename Dateipfad zur Geräte Datei, z.B. "/dev/i2c-0"
*/
void gnublin_module_lm75::setDevicefile(std::string filename){
	i2c.setDevicefile(filename);
}


//-----------------------------------get Temp-----------------------------------
/** @~english
* @brief reads the raw data via i2c from the LM75 chip and calculates the temperature
*
* @return Returns the Temperature, 0 at failure (check with fail() and getErrorMessage())
*
* @~german
* @brief liest die Roh-Daten aus dem LM75 und berechnet die Temperatur
*
* @return Temperatur, im Fehlerfall 0 (überprüfen mit fail() und getErrorMessage())
*/
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
/** @~english
* @brief reads the raw data via i2c from the LM75 chip and calculates the temperature
*
* @return Returns the Temperature as float, 0 at failure (check with fail() and getErrorMessage())
*
* @~german
* @brief liest die Roh-Daten aus dem LM75 und berechnet die Temperatur
*
* @return Temperatur als Fließkommazahl, im Fehlerfall 0 (überprüfen mit fail() und getErrorMessage())
*/
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
/** @~english
* @brief reads the raw data via i2c from the LM75 chip and shift the bits correctly
*
* @return Returns raw value, already shifted. 0 at failure (check with fail() and getErrorMessage())
*
* @~german
* @brief liest die Roh-Daten aus dem LM75 und schiebt die Bits in die richtige Reihenfolge
*
* @return Rohwert, im Fehlerfall 0 (überprüfen mit fail() und getErrorMessage())
*/
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
