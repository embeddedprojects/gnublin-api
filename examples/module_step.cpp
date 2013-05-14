#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <ios>
#include "gnublin.h"

gnublin_module_step Motor;


int main () {

//SetMotorParam
Motor.setVmax(4);
Motor.setMotorParam();      	
//GetFullStatus1 command:  This Command must be executed before Operating
  	Motor.getFullStatus1();
//RunInit command:  This Command must be executed before Operating
  	Motor.runInit();
  	Motor.drive(10000);
while(Motor.getMotionStatus()!=0){	//wait until Motor stands still
	usleep(10000);
}
Motor.drive(-10000);
}
