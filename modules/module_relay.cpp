#include "module_reley.h""


//****************************************************************************
// Class for easy use of the GNUBLIN Module-Relay
//****************************************************************************

gnublin_module_relay::gnublin_module_relay(int adr) {
	int i;
	pca9555 = new gnublin_module_pca9555(adr);
	for (i=0; i<=7; i++) {
		pca9555->pinMode(i, OUTPUT);
	}
}

int gnublin_module_relay::switchPin(int pin, int value) {
	if (pin < 0 || pin > 7) {
		return -1;
	}
	if (pca9555->digitalWrite(pin, value) < 0) {
		return -1;
	}
	return 1;
}



//-------------get Error Message-------------
// get the last ErrorMessage
// parameters:		NONE
// return:		[const char*]ErrorMessage	Error Message as c-string

const char *gnublin_module_relay::getErrorMessage(){
	return ErrorMessage.c_str();
}

