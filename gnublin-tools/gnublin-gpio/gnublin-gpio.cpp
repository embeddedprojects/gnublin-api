#include "../../gnublin.h"

using namespace std;

int c,hflag;
int json_flag = 0;
int brute_flag = 0;
int pin=-1;
bool input=false;
bool output=false;
int output_value=0;
bool unexport=false;

gnublin_gpio gpio;

void parse_opts(int argc, char **argv)
{

        while((c = getopt(argc,argv,"hjbifp:o:")) != -1)
        {
                switch(c)
                {
                        case 'h' : hflag = 1;                               	break;                              /* help */
                        case 'j' : json_flag = 1;                           	break;
                        case 'b' : brute_flag = 1;                          	break;
			case 'p' : pin=atoi(optarg);		    		break;
			case 'i' : input=true;					break;
				   output=false;
			case 'o' : output=true;
				   input=false;
				   output_value = atoi(optarg);			break;
			case 'f' : unexport=true;
				   output=false;
				   input=false;					break;
		}

        }
        if (hflag | (argc == 1))
        {
                printf("This program is designed, to easily interact with the GPIOs of GNUBLIN.\n\n"
                	"-h Show this help\n"
			"-b show output in raw format.\n"
			"-j Convert output to json format\n"
			"-p <value> Select gpio Pin (default=14) <value> between 1 and 20\n"
			"-i configure pin as input and print the value\n"
			"-o <value> configure pin as output and set a value 1=high,0=low\n"
			"-f unexport the gpio Pin\n\n"
                	"Examples:\n\n"
                	"\tset GPIO14 Pin as input, read value and print it in json format:\n"
			"\tgnublin-gpio -i -p 14 -j\n\n"
			"\tset GPIO11 Pin as output with a high value:\n"
			"\tgnublin-gpio -o 1 -p 11\n\n"
			"\tset GPIO11 Pin as output with a low value:\n"
			"\tgnublin-gpio -o 0 -p 11\n");              
        	exit(0);
        }
}



int main (int argc, char **argv) {
	int input_value=-1;
    	parse_opts(argc, argv);
    	if(input){
    		if(gpio.pinMode(pin, INPUT)!=1){
    			cout << gpio.getErrorMessage() << endl;
    			return -1;
    		}
    		if((input_value=gpio.digitalRead(pin))==-1){
    			cout << gpio.getErrorMessage() << endl;
    			return -1;
    		}
    		else {
    			if(json_flag){
    				cout << "{ \"pin\" : \"" << pin << "\", \"value\" : \""<< input_value << "\" }" << endl;
    			}
    			else if(brute_flag){
    				cout << input_value << endl;
    			}
    			else{
    				cout << "Value of Pin " << pin << " is " << input_value << endl;
    			}
    		}
    	}
    	else if(output){
    		if(gpio.pinMode(pin, OUTPUT)!=1){
    			cout << gpio.getErrorMessage() << endl;
    			return -1;
    		}
    		if((input=gpio.digitalWrite(pin, output_value))==-1){
    			cout << gpio.getErrorMessage() << endl;
    			return -1;
    		}
    		else{
    			if(json_flag){}
    			else if(brute_flag){}
    			else
    				cout << "GPIO" << pin << " is now output with value: " << output_value << endl;
    		}
    	}
    	else if(unexport){
    		if(gpio.unexport(pin)!=1){
    			cout << gpio.getErrorMessage() << endl;
    			return -1;
    		}
    	}
    	else{
    		cout << "Please correct your input!" << endl;
    		return -1;
    	}	
    		
    	return 0;
}












