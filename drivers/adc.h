#include "../include/includes.h"


//****************************************************************************
// Class for easy acces to the GPAs
//****************************************************************************

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
