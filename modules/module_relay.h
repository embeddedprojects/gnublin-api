#include "../include/includes.h"
#include "module_pca9555.cpp"


//****************************************************************************
// Class for easy use of the GNUBLIN Module-Relay
//****************************************************************************

class gnublin_module_relay {
		gnublin_module_pca9555 pca9555;
		bool error_flag;
		std::string ErrorMessage;
	public:
		gnublin_module_relay();
		const char *getErrorMessage();
		bool fail();
		void setAddress(int Address);
		void setDevicefile(std::string filename);
		int switchPin(int pin, int value);
};

