#include "../include/includes.h"
//*******************************************************************
//Class for accessing GNUBLIN i2c Bus
//*******************************************************************
/**
* @class gnublin_i2c
* @~english
* @brief Class for accessing GNUBLIN i2c bus
*
* The GNUBLIN I2C bus can easily accessed with this class
* @~german 
* @brief Klasse für den zugriff auf den GNUBLIN I2C Bus
*
* Die GNUBLIN I2C Klasse gewährt einfachen Zugriff auf den I2C Bus
*/ 

class gnublin_driver {
	bool error_flag;
	std::string ErrorMessage;
	virtual void onError() = 0;
protected:
	gnublin_driver();
public:
	bool fail();
	int setErrorMessage(std::string message);
	const char *getErrorMessage();
        void clearError();
};
