#include "gnublin.h"

int main(){
	gnublin_module_dac dac;
	
	/*
	dac.write(0, 1024);
	dac.write(1, 1024);
	dac.write(2, 1024);
	dac.write(3, 1024);

	dac.vRefEeprom(0, 1);
	dac.vRefEeprom(1, 1);
	dac.vRef(2, 1);
	dac.vRef(3, 1);
	dac.gainEeprom(0, 1);
	dac.gainEeprom(1, 0);
	dac.gain(2, 1);
	dac.gain(3, 0);
	*/

	printf("\n%i\n%i\n%i\n%i\n",dac.read(0), dac.read(1), dac.read(2), dac.read(3));
	//dac.writeAll(0, 1000, 1500, 2000);
	
	dac.read(0);
	dac.read(1);
	printf("%i", dac.read(2));
	dac.read(3);
}
