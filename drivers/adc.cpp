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
	std::ifstream file("/dev/lpc313x_adc");
	if (file.fail()) {
		system("modprobe lpc313x_adc");
		sleep(1);
	}
	file.close();
	clearError();
}

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
	if (file < 0) 
		return setErrorMessage("Unable to open "+device);
	file << pin_str;
	file.close();
	std::ifstream dev_file(device.c_str());
	dev_file >> value;
	dev_file.close();
	clearError();
	return hexstringToNumber(value);
}

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
	int value = getValue(pin);
	value = value*825/256;
	return value;
}

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
	clearError();
	return 1;
}



#endif
