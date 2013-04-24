#include "../include/includes.h"


//Hinweis: evtl. Fehlerhaft nicht getestet
//*****************************************************************************
// Class for accesing GNUBLIN Module-ADC / ADS7830
//*****************************************************************************

class gnublin_module_adc {
	public:
		gnublin_module_adc();
		gnublin_module_adc(int adr);
		gnublin_module_adc(int bus, int adr);
		int getValue(int channel);
		const char *getErrorMessage();
	private:
		std::string adress;
		std::string i2cbus;
		std::string ErrorMessage;
};
