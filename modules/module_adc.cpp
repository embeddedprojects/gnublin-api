#include "module_adc.h"


//Hinweis: evtl. Fehlerhaft nicht getestet
//*****************************************************************************
// Class for accesing GNUBLIN Module-ADC / ADS7830
//*****************************************************************************

gnublin_module_adc::gnublin_module_adc() {
	system("modprobe ads7828 >/dev/null");
	adress = "0x48";
	i2cbus = "1";
}

//-------------get Error Message-------------
// get the last ErrorMessage
// parameters:		NONE
// return:		[const char*]ErrorMessage	Error Message as c-string

const char *gnublin_module_adc::getErrorMessage(){
	return ErrorMessage.c_str();
}

gnublin_module_adc::gnublin_module_adc(int adr){
	adress = numberToString(adr);
	system("modprobe ads7828 >/dev/null");
	i2cbus = "1";
}


gnublin_module_adc::gnublin_module_adc(int bus, int adr) {
	i2cbus = numberToString(bus);
	adress = numberToString(adr);
	system("modprobe ads7828 >/dev/null");
}


int gnublin_module_adc::getValue(int channel) {
	if (channel > 7 || channel < 0){
		return -1;
	}
	std::string value;
	std::string channel_str = numberToString(channel);
	std::string device = "/sys/module/ads7828/drivers/i2c:ads7828/" + i2cbus + "-00" + adress.erase(0, 2) + "/in" + channel_str + "_input";
	std::ifstream file(device.c_str());
	file >> value;
	file.close();
	return stringToNumber(value);
}
