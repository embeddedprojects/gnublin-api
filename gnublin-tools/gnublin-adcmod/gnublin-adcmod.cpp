/*
* Author: Felix Steinberger
*
* Version 0.1
* embedded projects GmbH
*
* Discription: Application to read values from the ADC-Driver ADS7830 in mV
*
*/

#include <gnublin.h>

using namespace std;

gnublin_module_adc adc;
int hflag = 0;
int bare_flag = 0;
int json_flag = 0;
int dif_flag = 0;
int address = 0x48;
int channel = 1;


int error_msg(char* msg){
	if (json_flag) {
		printf("{\"error_msg:\" : \"%s not successful\", \"result\" : \"-1\",}\n", msg);
	}
	else {
		printf("Error: %s\n", msg);
	}
	exit (1);
}



void pars_opts(int argc, char **argv) {
	int c;
	while((c = getopt(argc,argv,"hc:bja:d")) != -1){
                switch(c)
                {
                        case 'h' : hflag = 1; break; /* help */
			case 'c' : channel = atoi(optarg); break;
			case 'b' : bare_flag = 1; break;
			case 'j' : json_flag = 1; break;
			case 'a' : address = atoi(optarg); break;
			case 'd' : dif_flag=1; break;
		}
	}

	if (hflag | (argc<=1)){
		printf("This Script prints out the value of a selected adc-channel of the gnublin-adc-module \n\n"
		"-h Show this help\n"
		"-b show raw output\n"
		"-j Convert output to json format\n"
		"-c <x> Select ADC Channel (1 - 8 possible, default is 1) \n"
		"-d enables differential mesurement between pears, only possible [1,2], [3,4], [5,6] [7,8]\n"
		"     first channel is given by -c, second channel is selectet automaticaly\n"
		"-a <ADDRESS> I2C-Address of the module (default is 0x48)\n\n"

		"Example:\n\n"

		"channel=2, I2C-Address=0x48 print value in json format:\n"
		"gnublin-adcmod -j -a 0x48 -c 2 \n\n"

		"channel=7, I2C-Address=0x4A print value in normal format:\n"
		"gnublin-adcmod -a 0x4A -c 7\n\n");

		exit(1);
	}
}

int main(int argc, char **argv) {
	int value;

	pars_opts(argc, argv);

	adc.setAddress(address);
	if (adc.fail()){
		error_msg((char*)"setting address");
	}
	
	if (channel < 1 || channel > 8){
		error_msg((char*)"channel not supported");
	}
	if (dif_flag) {
		if (bare_flag){
			value = adc.getValue(channel, channel+1);
			if (adc.fail()) {
				error_msg((char*)"getting value");
			}
		}
		else {
			value = adc.getVoltage(channel, channel+1);
			if (adc.fail()) {
				error_msg((char*)"getting value");
			}
		}

	}
	else {
		if (bare_flag){
			value = adc.getValue(channel);
			if (adc.fail()) {
				error_msg((char*)"getting value");
			}
		}
		else {
			value = adc.getVoltage(channel);
			if (adc.fail()) {
				error_msg((char*)"getting value");
			}
		}
	}
	if (json_flag) {
		printf("{\"voltage\" : \"%i\", \"result\" : \"0\"}\n", value);
	}
	else if (bare_flag) {
		printf("%i\n", value);
	}
	else {
		printf("%i mV\n", value);
	}

}
