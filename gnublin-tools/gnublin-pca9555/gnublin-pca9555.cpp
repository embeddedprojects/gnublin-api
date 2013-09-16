#include <gnublin.h>

using namespace std;


gnublin_module_pca9555 pca;


int value, c, hflag=0;
int json_flag = 0;
int brute_flag = 0;
int input_flag = 0;
int readState_flag = 0;
int pin= -1;
int port= -1;

string helpstring="This program was designed to easily interact with the PCA9555 Portexpander.\n\n"

"-h show this help\n"
"-a<X> Specify the Portexpander i2c address (default is 0x20)\n"
"-p<Y> Specify the portexpander pin (0-15)\n"
"-P<Z> Specify the Portexpander Port (0-1)\n"
"-o<value> Set pin or port as output with given <value>(0= low / 1 = high)\n"
"-i Set pin as input and read its value.\n"
"-j Convert Output to json Format\n"
"-b show output in raw format\n"
"-r show State of OUTPUT Pin\n\n"

"Examples:\n\n"

"Set pin 0 high \n"
"gnublin-pca9555 -p 0 -o 1\n\n"

"Set pin 1 of Port 1 low \n"
"gnublin-pca9555 -p 8 -o 0\n\n"

"Set Port 0 as output and as low\n"
"gnublin-pca9555 -P 0 -o 0\n\n"

"Read the State of OUTPUT Pin 3\n"
"gnublin-pca9555 -p 3 -r\n\n"

"Read the value of pin 3\n"
"gnublin-pca9555 -p 3 -i\n\n";


void parse_opts(int argc, char **argv)
{

	while((c = getopt(argc,argv,"ha:p:P:o:ijbr")) != -1)
	{
		switch(c)
		{
			case 'h' : 	printf("%s", helpstring.c_str());	
						hflag=1;							
																	break; // help 
			case 'a' : 	pca.setAddress(strtol (optarg,NULL,16)); 	break;
			case 'p' :	pin=atoi(optarg); 							break;
			case 'P' :	port=atoi(optarg); 							break;
			case 'o' :	value=atoi(optarg); 						break;
			case 'i' :	input_flag = 1;								break;
			case 'j' : 	json_flag = 1;								break;
			case 'b' : 	brute_flag = 1;								break;			
			case 'r' :	readState_flag = 1;
		}
	}
	if (hflag)
	{
	exit(0);
	}
}


int main (int argc, char **argv) {
	
	pca.setAddress(0x20);
	parse_opts(argc, argv);
	if(value >1){
		value=1;	
	}
		
	if (pin >= 0 && port >= 0 && input_flag== 0 && readState_flag == 0){
		pca.writePort(port, value*0xff);
		pca.digitalWrite(pin, value);
		pca.portMode(port, OUTPUT);
		pca.pinMode(pin, OUTPUT);
	}
	else if (pin >= 0 && port < 0 && input_flag== 0 && readState_flag == 0){
		pca.digitalWrite(pin, value);
		pca.pinMode(pin, OUTPUT);
	}
	else if (pin < 0 && port >= 0 && input_flag== 0 && readState_flag == 0){
		pca.writePort(port, value*0xff);
		pca.portMode(port, OUTPUT);
	}
	else if (pin >= 0 && port < 0 && input_flag== 1 && readState_flag == 0){
		pca.pinMode(pin, INPUT);
		if(json_flag==1){
			printf("{\"value\" : \"%i\",\"pin\" : \"%i\", \"result\" : \"0\"}\n", pca.digitalRead(pin), pin);
		}
		else if (brute_flag==1){
			printf("%i\n", pca.digitalRead(pin));
		}
		else printf("Value pin %i: %i\n", pin, pca.digitalRead(pin));
	}
	else if (pin >= 0 && port < 0 && input_flag == 0 && readState_flag == 1) {
		pca.pinMode(pin, OUTPUT);
    if(json_flag==1){
      printf("{\"value\" : \"%i\",\"pin\" : \"%i\", \"result\" : \"0\"}\n", pca.readState(pin), pin);
    }
    else if (brute_flag==1){
      printf("%i\n", pca.readState(pin));
    }
    else printf("Value pin %i: %i\n", pin, pca.readState(pin));
	}
	else if (pin >= 0 && port >= 0 && input_flag== 1 ){
		if(json_flag==1){
			printf("{\"error_msg\" : \"Error, can just read single pin not a whole port!\",\"result\" : \"-1\"}\n");
		}
		else if (brute_flag==1){
			printf("-1\n");
		}
		else printf("Error, can just read single pin not a whole port!\n");
	}
	else{
		if(json_flag==1){
			printf("{\"error_msg\" : \"Check your parameters!\",\"result\" : \"-1\"}\n");
		}
		else if (brute_flag==1){
			printf("-1\n");
		}
		else printf("Check your parameters!\n");
	}
	return 1;
}
