#include <gnublin.h>

using namespace std;


gnublin_module_relay relay;


int c,hflag=0;
int json_flag = 0;
int brute_flag = 0;
int pin=-1;
int value=-1;

string helpstring="This program was designed to easily interact with the relay module for GNUBLIN.\n\n"

"-h show this help\n"
"-p <Y> specify the relay number (1-8)\n"
"-o <value> set pin as output with given <value>(0= low / 1 = high)\n"
"-a <Address> specifiy the module address (default is 0x20)\n"
"-j show output in json format\n"
"-b show output in raw format\n\n"               

"Examples:\n"

"Set relay 1 high\n"
"gnublin-relay -p 1 -o 1\n\n"

"Set relay 1 on a Module connected to Address 0x21 low\n"
"gnublin-relay -a 0x21 -p 1 -o 0\n\n";

void parse_opts(int argc, char **argv)
{

	while((c = getopt(argc,argv,"hp:o:a:jb")) != -1)
	{
		switch(c)
		{
			case 'h' : 	printf("%s", helpstring.c_str());	
					hflag=1;							
												break; // help 
			case 'p' :	pin=atoi(optarg); 					break;
			case 'o' :	value=atoi(optarg); 					break;
			case 'a' : 	relay.setAddress(strtol (optarg,NULL,16));		break;
			case 'j' : 	json_flag = 1;						break;
			case 'b' : 	brute_flag = 1;						break;			
		}
	}
	if (hflag)
	{
	exit(0);
	}
}


int main (int argc, char **argv) {
//	relay.setAddress(0x20);
	parse_opts(argc, argv);


	if(relay.switchPin(pin, value)<0){
		if(json_flag ==1){
		printf("{ \"error_msg\" : \"%s\",\"result\" : \"-1\" }\n" ,relay.getErrorMessage());
		return -1;
		}
		else{
		printf("Error: %s\n", relay.getErrorMessage());
		return -1;
		}
	}
	return 1; 
}
