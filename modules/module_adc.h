#include "../include/includes.h"
#include "../drivers/i2c.h"

//*****************************************************************************
// Class for accesing GNUBLIN Module-ADC / ADS7830
//*****************************************************************************

/**
* @class gnublin_module_adc
* @~english
* @brief Class for accesing the GNUBLIN MODULE-ADC
*
* This class manages to use the GNUBLIN MODULE-ADC
* @~german
* @brief Klasse für den Zugriff auf das GNUBLIN MODULE-ADC
*
* Mit dieser Klasse kann auf das GNUBLIN MODULE-ADC zugegriffen werden.
*/
class gnublin_module_adc {
	public:
		gnublin_module_adc();
		int setAddress(int adr);
		int setDevicefile(std::string file);
		int setReference(int value);
		int getValue(int channel);
		int getValue(int channel1, int channel2);
		int getVoltage(int channel);
		int getVoltage(int channel1, int channel2);
		bool fail();
		const char *getErrorMessage();
	private:
		gnublin_i2c i2c;
		bool error_flag;
		std::string ErrorMessage;
		int reference_flag; // (1 = intern, 0 extern)
		int referenceValue;
};
