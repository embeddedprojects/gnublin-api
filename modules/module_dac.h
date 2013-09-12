#include "../include/includes.h"
#include "../drivers/i2c.cpp"

//*******************************************************************
//Class for accessing the MCP4728 digital to analog IC via I2C
//*******************************************************************
/**
* @class gnublin_module_dac
* @~english
* @brief Class for accessing the MCP4728 digital to analog IC via I2C
*
* @~german 
* @brief Klasse für den zugriff auf den MCP4728 digital zu analog Wandler 
*
*/
class gnublin_module_dac {
private:
	bool error_flag;
	gnublin_i2c i2c;
	int _channel[4];
	int _gain[4];
	int _vRef[4];
	std::string ErrorMessage;
public:
	gnublin_module_dac();
	const char *getErrorMessage();
	bool fail();
	void setAddress(int Address);
	int read(int);
	void gain(int channel, int val);
	void gainEeprom(int channel, int val);	
	void vRef(int channel, int val);
	void vRefEeprom(int channel, int val);
	void writeAll(int val_0, int val_1, int val_2, int val_3);
	void write(int channel, int value);										
	void writeEeprom(int channel, int value);
};
