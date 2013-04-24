#include "../include/includes.h"

//****************************************************************************
// Class for easy acces to the GPIOs
//****************************************************************************

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
