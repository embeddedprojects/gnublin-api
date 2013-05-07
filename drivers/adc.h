#include "../include/includes.h"

#if (BOARD != RASPBERRY_PI)
//****************************************************************************
// Class for easy acces to the GPAs
//****************************************************************************
/**
* @class gnublin_adc
* @~english
* @brief Class for easy acces to the GPAs
*
* With the gnublin_adc API you can access the GPAs of the GNUBLIN Board
* @~german 
* @brief Klasse für den zugriff auf die GPAs
*
* Mit der gnublin_adc API lassen sich die GPAs auf dem GNUBLIN einfach aus dem eigenem Programm heraus auslesen.  
*/
class gnublin_adc {
	public:
		gnublin_adc();
		int getValue(int pin);
		int getVoltage(int pin);
		int setReference(int ref);
		bool fail();
		const char *getErrorMessage();
	private:
		bool error_flag;
		std::string ErrorMessage;
};

#endif
