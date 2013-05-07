#include "../include/includes.h"

/**
* @class gnublin_gpio
* @~english
* @brief Class for accessing GNUBLIN GPIO-Ports
*
* With the gnublin_gpio API you can controll the GPIO-Ports of the GNUBLIN Board
* @~german 
* @brief Klasse für den zugriff auf die GPIO Pins
*
* Mit der gnublin_gpio API lassen sich die GPIO-Ports auf dem GNUBLIN einfach aus dem eigenem Programm heraus ansteuern.  
*/
class gnublin_gpio {
	public:
		gnublin_gpio();
		bool fail();
		int pinMode(int pin, std::string direction); //Defines GPIO<pin> as INPUT or OUTPUT
		int digitalWrite(int pin, int value); //Writes value on GPIO
		int digitalRead(int pin); //Reads value from GPIO<pin>
		int unexport(int pin);
		const char *getErrorMessage();
	private:
		bool error_flag;
		std::string ErrorMessage;
};
