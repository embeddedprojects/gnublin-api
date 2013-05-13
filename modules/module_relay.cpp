#include "module_relay.h""


//****************************************************************************
// Class for easy use of the GNUBLIN Module-Relay
//****************************************************************************


//------------------Konstruktor------------------
/** @~english 
* @brief Set standards.
*
* Set standard i2c address 0x20, set ErrorFlag false
* @~german 
* @brief Setze Standartwerte.
*
* Setze standard i2c Adresse 0x20 und setze das ErrorFlag auf false.
*/
gnublin_module_relay::gnublin_module_relay() {
	error_flag=false;
}


//-------------getErrorMessage-------------
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
const char *gnublin_module_relay::getErrorMessage(){
	return ErrorMessage.c_str();
}

//-------------------------------fail-------------------------------
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
bool gnublin_module_relay::fail(){
	return error_flag;
}

//-------------setAddress-------------
/** @~english 
* @brief Set the slave address 
*
* With this function you can set the individual I2C Slave-Address of the module.
* @param Address new I2C slave Address
*
* @~german 
* @brief Setzt Slave Adresse.
*
* Mit dieser Funktion kann die individuelle I2C Slave-Adresse des Moduls gesetzt werden.
* @param Address neue I2C slave Adresse
*/
void gnublin_module_relay::setAddress(int Address){
	pca9555.setAddress(Address);
}


//-------------setDevicefile-------------
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
void gnublin_module_relay::setDevicefile(std::string filename){
	pca9555.setDevicefile(filename);
}

//-------------------switch Pin----------------
/** @~english 
* @brief Switch pin.
*
* This function sets the given relay to the given value.
* @param pin Number of the relay to switch (1-8)
* @param value close (1) or open (0) the relay (ON, OFF)
* @return success: 1, failure: -1
*
* @~german 
* @brief Schalte Pin.
*
* Schalten der einzelnen Relays. 
* @param pin Nummer des anzusteuernden Relays (1-8)
* @param value schließen (1) oder öffnen (0) des Relays (ON, OFF)
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_relay::switchPin(int pin, int value) {
	error_flag=false;

	if (pin < 1 || pin > 8) {
		error_flag=true;
		ErrorMessage="pin is not between 1-8!\n";
		return -1;
	}
	if (pca9555.pinMode((pin-1), OUTPUT) < 0){
		error_flag=true;
		ErrorMessage=pca9555.getErrorMessage(); //"pca9555.pinMode failed! Address correct?\n";
		return -1;
	}
	if (pca9555.digitalWrite((pin-1), value) < 0) {
		error_flag=true;
		ErrorMessage="pca9555.digitalWrite failed! Address correct?\n";		
		return -1;
	}
	return 1;
}


