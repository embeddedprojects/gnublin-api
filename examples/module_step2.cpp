#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <ios>

#define BOARD RASPBERRY_PI
#include "gnublin.h"

gnublin_module_step Motor;


int main () {

int pos= -10000, n=0, s;

// Motor.setAddress( 0x60 );
Motor.setAddress( 0x61 );
Motor.setVmax(4); //SetMotorParam
Motor.setMotorParam( 3,3,3,3 );
//GetFullStatus1 command: This Command must be executed before Operating
Motor.getFullStatus1();

//RunInit command: This Command must be executed before Operating

printf(" Init %d\n", Motor.runInit());
sleep(1);
printf(" Reset_pos %d\n", Motor.resetPosition());
printf(" Reset_pos %d\n", Motor.resetPosition());
printf(" Act_Pos: %d\n", Motor.getActualPosition());
fflush(stdout);

Motor.drive(pos);

while( (s=Motor.getMotionStatus()) !=0 ) // wait until Motor stands still
{
if( ++n > 80 ) break;
printf("%d ",s); fflush(stdout);
usleep(50000);
}
Motor.softStop();
printf("\n Act_Pos: %d Stop %d\n", Motor.getActualPosition(),n);

Motor.drive(-pos);
sleep(2);
printf(" Act_Pos: %d\n", Motor.getActualPosition());
}
