#include "../include/includes.h"
//*******************************************************************
//Class for accessing GNUBLIN i2c Bus
//*******************************************************************

class gnublin_i2c {
	bool error_flag;
	int slave_address;
	std::string devicefile;
	std::string ErrorMessage;
public:
	gnublin_i2c();
	bool fail();
	void setAddress(int Address);
	int getAddress();
	const char *getErrorMessage();
	void setDevicefile(std::string filename);
	int receive(char *RxBuf, int length);
	int receive(unsigned char RegisterAddress, char *RxBuf, int length);
	int send(char *TxBuf, int length);
	int send(unsigned char RegisterAddress, char *TxBuf, int length);
	int send(int value);
};
