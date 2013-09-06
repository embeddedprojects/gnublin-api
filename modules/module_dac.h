#include "../include/includes.h"
#include "../drivers/i2c.cpp"

//*******************************************************************
//Class for accessing the LM75 IC via I2C
//*******************************************************************
/**
* @class gnublin_module_lm75
* @~english
* @brief Class for accessing the LM75 IC via I2C
*
* @~german 
* @brief Klasse für den zugriff auf den LM75 IC via I2C Bus
*
*/
class gnublin_module_dac {
private:
	//bool error_flag;
	gnublin_i2c i2c;
	int _channel[4];
	//std::string ErrorMessage;
public:
	gnublin_module_dac();
	//const char *getErrorMessage();
	//bool fail();
	void setAddress(int Address);
	//int* read(int);
	
	void write(int channel, int value);

};
