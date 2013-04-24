#include "../include/includes.h"
#include "../drivers/i2c.cpp"


class gnublin_module_step {
	gnublin_i2c i2c;

	unsigned int irun;
	unsigned int ihold;
	unsigned int vmax;
	unsigned int vmin;
	std::string ErrorMessage;
public:
	gnublin_module_step();
	void setAddress(int Address);
	void setDevicefile(std::string filename);
	int setIrun(unsigned int newIrun);
	int setIhold(unsigned int newIhold);
	int setVmax(unsigned int newVmax);
	int setVmin(unsigned int newVmin);
	int writeTMC(char *buffer, int num);
	int readTMC(char *RxBuf, int num);
	int burnNewAddress(int new_address);
	int getFullStatus1();
	int getFullStatus2();
	int runInit();
	int setMotorParam();
	int setMotorParam(unsigned int newIrun, unsigned int newIhold, unsigned int newVmax, unsigned int newVmin);
	int hardStop();
	int softStop();
	int resetPosition();
	int setPosition(int position);
	int getSwitch();
	int getActualPosition();
	int drive(int steps);
	int getMotionStatus();
	const char *getErrorMessage();
};
