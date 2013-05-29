#include "gnublin.h"
#include <signal.h>



gnublin_module_dogm display;
gnublin_gpio gpio;
gnublin_adc adc;


using namespace std;

void my_handler(int s){
	gpio.pinMode(18, OUTPUT);
	gpio.digitalWrite(18, LOW);
	display.clear();
	display.print((char*)"Heizung: aus", 2);
	exit(1); 
}

float get_temperature(){
	int voltage =0;
	float resistance=0;
	float temperature=0;
	
	
	if((voltage = adc.getVoltage(1))==-1){
		return -1;
	}
	else {
		resistance = (voltage * 330) / (3300 - voltage);
		temperature = ((resistance/100)-1)*259.74;
	}
	return temperature;
}

int main(int argc, char **argv){

	int initial=1;
	char tempchar[13];
	float temperature;
	int heating=0;
	signal (SIGINT,my_handler);
	
	gpio.pinMode(18, OUTPUT);
	gpio.digitalWrite(18, HIGH);
		
	while(1){
		
		temperature = get_temperature();
		if((temperature < 900) & (temperature > -100)){
			sprintf(tempchar, "Temp: %.2f", temperature);
			display.clear();
			display.controlDisplay(1,0,0);
			display.print(tempchar);
			if(temperature < 230){
				gpio.digitalWrite(18, HIGH);
				heating = 1;
			}
			else{
				if(temperature > 240){
					if(initial){
						initial = 0;
						system("gnublin-pwm -v 0x400");
						sleep(1);
						system("gnublin-pwm -v 0x000");
					}
					gpio.digitalWrite(18, LOW);
					heating = 0;
				}
			}
			if(heating)display.print((char*)"Heizung: ein", 2);
			if(!heating)display.print((char*)"Heizung: aus", 2);
		}
		sleep(10);
    	}


	return 1;
}
