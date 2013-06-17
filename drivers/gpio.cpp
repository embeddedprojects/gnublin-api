#include "gpio.h"
#include "driver.h"

/** @~english 
* @brief Reset the ErrorFlag.
*
* @~german 
* @brief Setzt das ErrorFlag zurück.
*
*/
gnublin_gpio::gnublin_gpio() : gnublin_driver() {
}



/** @~english 
* @brief Removes the GPIO. 
*
* Removes the GPIO from the filesystem, after that, you cannot access the pin.
* @return bool error_flag
*
* @~german 
* @brief Entferne GPIO
*
* Entfernt den GPIO aus dem Filesystem, es kann kein Zugriff mehr auf diesen erfolgen.
* @return bool error_flag
*/
int gnublin_gpio::unexport(int pin){
	std::string pin_str = numberToString(pin);
	std::string dir = "/sys/class/gpio/unexport";
	std::ofstream file (dir.c_str());
	if (file < 0) 
		return setErrorMessage("Unable to open " + dir +"\n");
	file << pin_str;
	file.close();
	clearError();
	return 1;
}


/** @~english 
* @brief Change the PinMode.
*
* With this function you can set the pin as input or output. <br>
* This must be done before all other operations. <br><br>
* Hint: GPIO 4 can only be used as output.
* @param pin Pin number to change the mode
* @param direction direction of the pin (OUTPUT, INPUT)
* @return success: 1, failure: -1
*
* @~german 
* @brief Pin Modi festlegen.
*
* Mit dieser Funktion kann der Modi des Pins festgelegt werden, also ob er als Eingang oder Ausgang genutzt werden soll. <br>
* Vor allen anderen Zugriffen muss diese Funktion ausgeführt werden. <br> <br>
* Hinweis: GPIO 4 kann auf dem GNUBLIN board nur als Ausgang genutzt werden. 
* @param pin Nummer des GPIO-Pins
* @param direction Richtung des Pins (OUTPUT, INPUT) 
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_gpio::pinMode(int pin, std::string direction){
	#if (BOARD != RASPBERRY_PI)
	if (pin == 4 && direction == "out")
                return setErrorMessage("Pin 4 can not be set to 'out'\n");
	#endif
	std::string pin_str = numberToString(pin);
	std::string dir = "/sys/class/gpio/export";
	std::ofstream file (dir.c_str());
	if (file < 0) 
                return setErrorMessage("Unable to open " + dir + "\n");
	file << pin;
	file.close();

	dir = "/sys/class/gpio/gpio" + pin_str + "/direction";

	file.open(dir.c_str());
	if (file < 0) 
                return setErrorMessage("Unable to open " + dir + "\n");
	file << direction;
	file.close();

	clearError();
	return 1;
}


/** @~english 
* @brief Write Pin.
*
* Set the Pin to the given value. 
* @param pin Pin number to change the mode
* @param value Value of the Pin (LOW, HIGH)
* @return success: 1, failure: -1
*
* @~german 
* @brief Pin schreiben.
*
* Den GPIO-Pin auf einen bestimmten Wert setzen. 
* @param pin Nummer des entsprechenden GPIO-Pins
* @param value Wert auf den der Pin gesetzt werden soll (LOW, HIGH) 
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_gpio::digitalWrite(int pin, int value){
	#if (BOARD != RASPBERRY_PI)
	if (pin == 4)
                return setErrorMessage("Pin 4 is not available for usage\n");
	#endif
	if (value != 0 && value != 1)
                return setErrorMessage("Value ["+numberToString(value)+"] must be 0 or 1\n");
	std::string value_str = numberToString(value);
	std::string pin_str = numberToString(pin);
	std::string dir = "/sys/class/gpio/gpio" + pin_str + "/value";

	std::ofstream file (dir.c_str());
	if (file < 0) 
                return setErrorMessage("Unable to open " + dir + "\n");
	file << value_str;
	file.close();
	clearError();
	return 1;
}

/** @~english 
* @brief Read Pin.
*
* Reads the value of the given pin. 
* @param pin Pin number to read
* @return Value of the GPIO-Pin (0,1), -1 in case of failure 
*
* @~german 
* @brief Pin lesen.
*
* Abfragen des GPIO-Wertes. 
* @param pin Nummer des entsprechenden GPIO-Pins
* @return Wert des GPIO-Pins (0,1), -1 im Fehlerfall 
*/
int gnublin_gpio::digitalRead(int pin) {
	std::string value;
	
	std::string pin_str = numberToString(pin);
	std::string device = "/sys/class/gpio/gpio" + pin_str + "/value";
	std::ifstream file(device.c_str());
	if (file < 0)
                return setErrorMessage("Unable to open " + device + "\n");
	file >> value;
	file.close();
	clearError();
	return stringToNumber(value);
	
}

