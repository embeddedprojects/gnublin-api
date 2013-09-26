#include "gnublin.h"

int main(){
	gnublin_module_dac dac;
	
	// Write to all to specif channels	
	dac.writeEeprom(0, 4095); //Writes value to register and EEPROM
	dac.writeEeprom(1, 2048);
	dac.write(2, 1024);				//Writes value only to register
	dac.write(3, 0);

	dac.vRefEeprom(0, 1);			//Writes vRef condition also to EEPROM
	dac.vRefEeprom(1, 1);			
	dac.vRef(2, 1);						//Channel two will get internal Reference. 0 would be external
	dac.vRef(3, 1);
	dac.gainEeprom(0, 1);			//Writes gain condition also to EEPROM
	dac.gainEeprom(1, 0);
	dac.gain(2, 1);						//2x Gain. Voltage Reference doubled. Max is External voltage
	dac.gain(3, 0);						//1x Gain. Voltage Reference is vRef.
	
	//Reads all Channels:
	printf("\n%i\n%i\n%i\n%i\n",dac.read(0), dac.read(1), dac.read(2), dac.read(3));
	
	//Uncomment this for writing to all channels at once
	//dac.writeAll(0, 1000, 1500, 2000);
}
