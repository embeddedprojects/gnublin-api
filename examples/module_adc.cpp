#include "gnublin.h"

int main(){
	gnublin_module_adc adc;

	adc.setReference(IN);
	
	while(1){
		int value = adc.getValue(1, 2);
		int voltage = adc.getVoltage(1, 2);
		printf("Value: %i \n", value);
		printf("Voltage: %i \n\n", voltage);
		sleep(1);
	}

}
