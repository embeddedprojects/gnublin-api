#include <stdlib.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <ios>
#include <gnublin.h>

using namespace std;


gnublin_module_lm75 lm75;
int c,hflag=0;
int json_flag = 0;
int brute_flag = 0;
string helpstring="\nThis program is designed, to easily read the temperature from a LM75 Temp Sensor connected to the GNUBLIN.\n\n\r"
"-h Show this help\n"
"-j Convert output to json format\n"
"-b show output in raw format\n"
"-a set slave-address e.g. 0x48\n\n\r"
"Examples:\n\n"
"read temperature default I2CAddress=0x4f  print value in json format:\n"
"gnublin-lm75 -j\n\n"
"read temperature default I2CAddress=0x4f. print the value in raw format:\n"
"gnublin-lm75 -b\n\n"
"read temperature I2CAddress=0x48. print the value:\n"
"gnublin-lm75 -a 0x48\n\n\r";


void parse_opts(int argc, char **argv)
{

	while((c = getopt(argc,argv,"hjba:")) != -1)
	{
		switch(c)
		{
			case 'h' : hflag = 1;		break;	// help 
			case 'j' : json_flag = 1;	break;
			case 'b' : brute_flag = 1;	break;
			case 'a' : lm75.setAddress(strtol (optarg,NULL,16)); break;
		}

	}
	if (hflag)
	{
		cout << helpstring << endl;
	exit(1);
	}
}


int main (int argc, char **argv) {
	float temperature=9999; // LM75 goes from -55 to +125 °C
				// so if you get 9999 °C something went wrong

	lm75.setAddress(0x4f);
	parse_opts(argc, argv);

    if (argc == 0) {
		cout << helpstring << endl;
	exit(1);
	}
	
	temperature=lm75.getTempFloat();
	if(!lm75.fail()){
		if (json_flag == 1)
			cout << fixed << setprecision(3) << "{\"temperature\" : \"" << temperature << "\", \"result\" : \"0\"}" << endl;
		else if (brute_flag ==1){
			temperature=lm75.getTemp();
			cout << temperature << endl;
		}
		else
		cout << fixed << setprecision(3) << temperature << " C" << endl;
	}
	else{
		if (json_flag == 1){
			cout << "{\"error_msg\" : \"Failed to read from i2c device Address correct?\",\"result\" : \"-1\"}" << endl;
			return -1;
		}
		else if (brute_flag ==1){
			cout << "error failed to read from i2c device. Address correct?" << endl;
			return -1;
		}
		else{
			cout << "error failed to read from i2c device. Address correct?" << endl;
			return -1;
		}
	}
}	
