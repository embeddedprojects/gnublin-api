#include "adc.h"

#if (BOARD != RASPBERRY_PI)
//****************************************************************************
// Class for easy acces to the GPAs
//****************************************************************************
/** @~english 
* @brief Opens the file stream and loads the kernel object lpc313x_adc.
*
* @~german 
* @brief Erzeugt den Datei stream und lädt das Kernelmodul lpc313x_adc.
*
*/
gnublin_adc::gnublin_adc(){
	#if (BOARD == BEAGLEBONE_BLACK)
	std::ifstream file("/sys/devices/ocp.2/helper.14/AIN1");
	if (file.fail()) {
		std::ofstream file2("/sys/devices/bone_capemgr.14/slots");
		file2 << "cape-bone-iio";
		file2.close();
		sleep(1);
	}
	#else
	std::ifstream file("/dev/lpc313x_adc");
	if (file.fail()) {
		system("modprobe lpc313x_adc");
		sleep(1);
	}
	#endif
	file.close();
	error_flag = false;
}

//-------------fail-------------
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
bool gnublin_adc::fail(){
	return error_flag;
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
const char *gnublin_adc::getErrorMessage(){
	return ErrorMessage.c_str();
}

#if (BOARD != BEAGLEBONE_BLACK)
//-------------getValue-------------
/** @~english 
* @brief Get Value.
*
* This Funktion returns the Value from the register of the given pin.
* @param pin The pin, which should be used
* @return Value of the Pin - in failure: -1
*
* @~german 
* @brief Gibt den Wert des Pins zurück.
*
* Mit dieser Funktion erhält man den gemessenen Rohwert des angegebenen Pins, also den Wert des Registers. 
* @param pin Gibt den ADC-Pin an, von dem gemessen werden soll 
* @return Wert des ADCs, im Fehlerfall -1
*/
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
#endif


//-------------getVoltage-------------
/** @~english 
* @brief Get Voltage.
*
* This Funktion returns the Voltage of the given pin.
* @param pin The pin, which should be used
* @return Voltage of the Pin - in failure: -1
*
* @~german 
* @brief Ließt Spannung.
*
* Liefert den gemessenen Wert in mV. 
* @param pin Gibt den ADC-Pin an, von dem gemessen werden soll 
* @return Spannung des ADCs in mV, im Fehlerfall -1
*/
int gnublin_adc::getVoltage(int pin){
	#if (BOARD == BEAGLEBONE_BLACK)
	int value;
	std::stringstream ss;
	ss << pin;
	std::string device = "/sys/devices/ocp.2/helper.14/AIN" + ss.str();
	for (int i=0; i<2; i++){
		std::ifstream dev_file(device.c_str());
		dev_file >> value;
		dev_file.close();
	}
	#else
	int value = getValue(pin);
	value = value*825/256;
	#endif
	return value;
}

#if (BOARD != BEAGELEBONE_BLACK)
//-------------setReference-------------
/** @~english 
* @brief set Reference.
*
* @param ref 
* @return success: 1, failure: -1
*
* @~german 
* @brief setzt Referenz.
*
* @param ref 
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_adc::setReference(int ref){
	error_flag = false;
	return 1;
}
#endif


#endif
