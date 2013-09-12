#include <stdlib.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <ios>
#include <gnublin.h>

using namespace std;


gnublin_module_dac dac;
int c,hflag=0;
int json_flag = 0;
int brute_flag = 0;
int _channel = -1;
int _value = 0;
int _write = 0;
int _eeprom = 0;
int _vRef = 0;
int _gain = 0;
string helpstring="\nThis program is designed, to easily access the GNUBLIN MODUL DAC \n(Digital to Analog Converter) MCP4728.\n\n\r"
"-h Show this help\n"
"-j Convert output to json format\n"
"-b show output in raw format\n"
"-a set slave-address e.g. 0x60\n"
"-c Select channel: 0-3. If not more is given, value of register will be read. Use -b and -j for other formats.\n"
"-v Select value. 12 bit resolution: 0-4095.\n"
"-e Write to EEPROM\n"
"-r Select Internal Referenz = 2.048V.\n"
"-g Set gain on 2x. Use -g only if you have selected Internal Ref (-r).\n\n\r"
"Examples:\n\n"
"simple wirte on channel 0 a specific value:\n"
"gnublin-dac -c 0 -v 4095\n\n\r"
"Write to channel 2, register and EEPROM and use internal ref with gain:\n"
"gnublin-dac -c 2 -v 4095 -e -r -g\n\n\r"
"read value of channel 2:\n"
"gnublin-dac -c 2\n\n\r"
;


void parse_opts(int argc, char **argv)
{

	while((c = getopt(argc,argv,"hjba:c:v:erg")) != -1)
	{
		switch(c)
		{
			case 'h' : hflag = 1;		break;	// help 
			case 'j' : json_flag = 1;	break;
			case 'b' : brute_flag = 1;	break;
			case 'a' : dac.setAddress(strtol (optarg,NULL,16)); break;
			case 'c' : _channel=atoi(optarg); break;
			case 'v' : _value=atoi(optarg); _write = 1; _eeprom = 0; _vRef = 0; _gain = 0; break;
			case 'e' : _eeprom = 1; break;
			case 'r' : _vRef = 1; break;
			case 'g' : _gain = 1; break;
		}

	}
	if (hflag)
	{
		cout << helpstring << endl;
	exit(1);
	}
}


int main (int argc, char **argv) {


	//lm75.setAddress(0x4f);
	parse_opts(argc, argv);

    if (argc == 0) {
		cout << helpstring << endl;
	exit(1);
	}
	/*
	//temperature=lm75.getTempFloat();
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
*/
	if (_channel == -1 || _channel > 3) cout << "Error: Use -c to select Channel. -c 0..3" << endl;

	else if ((_channel >= 0) && !_write && !_eeprom && !_vRef && !_gain && !brute_flag && !json_flag) {
		cout << "Value of channel " << _channel << " is " << dac.read(_channel) << "." << endl;
	} 
	else if ((_channel >= 0) && !_write && !_eeprom && !_vRef && !_gain && !brute_flag && json_flag) {
		cout << fixed << setprecision(3) << "{\"Channel\" : \"" << _channel << "\", \"Value\" : \"" << dac.read(_channel) << "\", \"result\" : \"0\"}" << endl;
	}
	else if ((_channel >= 0) && !_write && !_eeprom && !_vRef && !_gain && brute_flag && !json_flag) {
		cout << dac.read(_channel) << endl;
	}
	else if (!_vRef && _gain) {
		cout << "Error. Gain not possible if vRef is external. See Datasheet.\nUse -r to activate vRef internal." << endl;
		return -1;
	}

	else if ((_channel >= 0) && _write && !_eeprom && !_vRef && !_gain) {
		dac.write(_channel, _value);
	}

	else if ((_channel >= 0) && _write && _eeprom && !_vRef && !_gain) {
		dac.writeEeprom(_channel, _value);
	}

	else if ((_channel >= 0) && _write && !_eeprom && _vRef && !_gain) {
		dac.write(_channel, _value);
		dac.vRef(_channel, 1);
	}

	else if ((_channel >= 0) && _write && !_eeprom && _vRef && _gain) {
		dac.write(_channel, _value);
		dac.vRef(_channel, 1);
		dac.gain(_channel, 1);
	}

	else if ((_channel >= 0) && _write && _eeprom && _vRef && !_gain) {
		dac.writeEeprom(_channel, _value);
		dac.vRefEeprom(_channel, 1);
	}

	else if ((_channel >= 0) && _write && _eeprom && _vRef && _gain) {
		dac.writeEeprom(_channel, _value);
		dac.vRefEeprom(_channel, 1);
		dac.gainEeprom(_channel, 1);
	} else {
		cout << "Error: Unkown command. Use -h for more information." << endl;
	}




	return 0;
}	
