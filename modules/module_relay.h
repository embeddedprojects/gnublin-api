#include "../include/includes.h"
#include "pca9555.cpp"


//****************************************************************************
// Class for easy use of the GNUBLIN Module-Relay
//****************************************************************************

class gnublin_module_relay {
	public:
		gnublin_module_relay(int adr);
		int switchPin(int pin, int value);
		const char *getErrorMessage();
	private:
		gnublin_module_pca9555* pca9555;
		std::string ErrorMessage;
};

