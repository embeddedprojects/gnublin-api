#include "gnublin.h"

int main(){
	gnublin_module_adc_12_bit adc;

	adc.setReference(0);
	
	while(1){
		int value = adc.getValue(1);
		if (adc.fail()) {
			printf("Error\n");
		}


		int voltage = adc.getVoltage(1, 2);
		printf("Value: %i \n", value);
		printf("Voltage: %i mV\n\n", voltage);
		sleep(1);
	}

}
