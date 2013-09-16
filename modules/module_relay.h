#include "../include/includes.h"
#include "module_pca9555.cpp"


//****************************************************************************
// Class for easy use of the GNUBLIN Module-Relay
//****************************************************************************
/**
* @class gnublin_module_relay
* @~english
* @brief Class for accessing GNUBLIN module-relay
*
* The GNUBLIN Module-relay can be easily controlled with the gnublin_step API. The Module uses the I2C-Bus.
* @~german 
* @brief Klasse für den zugriff auf das GNUBLIN module-relay
*
* Das GNUBLIN module-relay lässt sich mit Hilfe der gnublin_relay API ganz einfach ansteuern. Das Modul nutzt die I2C-Schnittstelle.  
*/ 
class gnublin_module_relay {
	gnublin_module_pca9555 pca9555;
	gnublin_i2c i2c;
	bool error_flag;
	std::string ErrorMessage;
public:
	gnublin_module_relay();
	const char *getErrorMessage();
	bool fail();
	void setAddress(int Address);
	void setDevicefile(std::string filename);
	int switchPin(int pin, int value);
	int readState(int pin);
};

