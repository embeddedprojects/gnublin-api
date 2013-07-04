#include "../include/includes.h"
#include "../drivers/spi.cpp"
#include "../drivers/gpio.cpp"

//***************************************************************************
// Class for accesing the GNUBLIN MODULE-DISPLAY 2x16
//***************************************************************************

/**
* @class gnublin_module_dogm
* @~english
* @brief Class for accesing the GNUBLIN MODULE-DISPLAY 2x16
*
* This class manages to display datas on the DOGM-Display.
* @~german
* @brief Klasse für den Zugriff auf das GNUBLIN MODULE-DISPLAY 2x16.
*
* Diese Klasse ermöglicht das Beschrieben des DOGM-Displays.
*/
class gnublin_module_dogm{
	public:
		gnublin_module_dogm();
		int init();
		bool fail();
		const char *getErrorMessage();
		int setRsPin(int pin);
		int setCS(int cs);
		int print(char* buffer);
		int print(char* buffer, int line);
		int print(char* buffer, int line, int off);
		int offset(int num);
		int clear();
		int returnHome();
		int shift(int num);
		int controlDisplay(int power, int cursor, int blink);
	private:
		gnublin_spi spi;
		gnublin_gpio gpio;
		bool error_flag;
		int rs_pin;
		std::string ErrorMessage;

};
