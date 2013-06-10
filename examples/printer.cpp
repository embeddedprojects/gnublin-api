#include "gnublin.h"
#include <signal.h>



gnublin_module_step Motor_x;
gnublin_module_step Motor_z;
gnublin_module_step Motor_y;
gnublin_module_step Motor_p;


using namespace std;

int hflag = 0;
int initflag = 0;
int printflag = 0;

void my_handler(int s){
	cout << "Execution aborted!" << endl;
	Motor_x.hardStop();
	Motor_z.hardStop();
	Motor_y.hardStop();
	Motor_p.hardStop();
	Motor_z.drive(10500);
	while(Motor_z.getMotionStatus()!=0){
		usleep(10000);	
	}
	exit(1);
}

int initMotor(){
	//set the Motor Slave Addresses
	Motor_x.setAddress(0x7f);
	Motor_z.setAddress(0x60);
	Motor_y.setAddress(0x62);
	Motor_p.setAddress(0x61);

	//Motor_x
	//SetMotorParam
	Motor_x.setVmax(4);
	Motor_x.setIrun(15);
	Motor_x.setMotorParam();      	
	//GetFullStatus1 command:  This Command must be executed before Operating
	Motor_x.getFullStatus1();
	//RunInit command:  This Command must be executed before Operating
	Motor_x.runInit();

	//Motor_z      	
	//SetMotorParam
	Motor_z.setVmax(14);
	Motor_z.setMotorParam();      	
	//GetFullStatus1 command:  This Command must be executed before Operating
	Motor_z.getFullStatus1();
	//RunInit command:  This Command must be executed before Operating
	Motor_z.runInit();

	//Motor_y      	
	//SetMotorParam
	Motor_y.setVmax(4);
	Motor_y.setIrun(15);
	Motor_y.setMotorParam();      	
	//GetFullStatus1 command:  This Command must be executed before Operating
	Motor_y.getFullStatus1();
	//RunInit command:  This Command must be executed before Operating
	Motor_y.runInit();

	//Motor_p      	
	//SetMotorParam
	Motor_p.setVmax(0);
	Motor_p.setMotorParam();      	
	//GetFullStatus1 command:  This Command must be executed before Operating
	Motor_p.getFullStatus1();
	//RunInit command:  This Command must be executed before Operating
	Motor_p.runInit();	
	return 1;
}
int initPrinter(){
	Motor_z.drive(30000);
	while(Motor_z.getMotionStatus()!=0){
		usleep(10000);
	}
	Motor_z.drive(30000);
	while(Motor_z.getMotionStatus()!=0){
		usleep(10000);
	}
	Motor_z.drive(30000);
	while(Motor_z.getMotionStatus()!=0){
		usleep(10000);
	}

	//drive all Motors to the Swich

	while(Motor_y.getSwitch()==0){
		Motor_y.drive(-10000);
		while(Motor_y.getMotionStatus()!=0){
			usleep(1000);
			if(Motor_y.getSwitch()){
				Motor_y.hardStop();
				Motor_y.resetPosition();
				break;
			}
		}
	}
	
	while(Motor_x.getSwitch()==0){
		Motor_x.drive(10000);
		while(Motor_x.getMotionStatus()!=0){
			usleep(1000);
			if(Motor_x.getSwitch()){
				Motor_x.hardStop();
				Motor_x.resetPosition();
				break;
			}
		}
	}

	while(Motor_z.getSwitch()==0){
		Motor_z.drive(-30000);
		while(Motor_z.getMotionStatus()!=0){
			usleep(1000);
			if(Motor_z.getSwitch()){
				Motor_z.hardStop();
				Motor_z.resetPosition();
				break;
			}
		}
	}
	
	//drive Motors to the print position
	Motor_z.drive(30000);
	while(Motor_z.getMotionStatus()!=0){
		usleep(10000);
	}
	Motor_z.drive(30000);
	while(Motor_z.getMotionStatus()!=0){
		usleep(10000);
	}
	Motor_z.drive(20000);
	while(Motor_z.getMotionStatus()!=0){
		usleep(10000);
	}

	Motor_y.drive(7300);
	while(Motor_y.getMotionStatus()!=0){
		usleep(10000);
	}

	Motor_x.drive(-7500);
	while(Motor_x.getMotionStatus()!=0){
		usleep(10000);
	}
	return 1;	
}

int print(){
	Motor_p.drive(-10);
	while(Motor_p.getMotionStatus()!=0)
		usleep(150);
	return 1;
}

int parse_opts(int argc, char **argv){
	int c;
	while((c = getopt(argc,argv,"hip")) != -1){
		switch(c){
			case 'h' : hflag = 1;					break;	/* help */
			case 'i' : initMotor(); initPrinter(); exit(1);		break;
			case 'p' : printflag = 1;				break;
		}

	}
	if (hflag | (argc<=1)){
		printf("This is the 3D-Printer alpha software!\n\n"
			"-h Show this help\n"
			"-i Initialize the Printer Position\n"
			"-p Print\n\n"
			"Examples:\n\n"
			"Initialize the printer position: \n\t%s -i\n"
			"Print: \n\t%s -p\n", argv[0], argv[0]);
		exit(1);
	}
	return 1;
}


int print_x_slow(int position, int steps)
{
	int i;

	for(i=0;i<=position;i+=abs(steps)){
		Motor_x.drive(steps);
		Motor_p.drive(-(abs(steps)/3));
		usleep(1000*3);
	}
	return 1;
}

int print_z_slow(int position, int steps)
{
	int i;

	for(i=0;i<=position;i+=abs(steps)){
		Motor_y.drive(steps);
		Motor_p.drive(-(abs(steps)/3));
		usleep(1000*3);
		
	}
	return 1; 
}

int main(int argc, char **argv){
	int i=0;
	signal (SIGINT,my_handler);
	parse_opts(argc, argv);

	initMotor();
	
	usleep(50000);    	
	Motor_x.setVmax(0);
	Motor_x.setMotorParam(); 
	Motor_y.setVmax(0);
	Motor_y.setMotorParam();

	Motor_z.drive(-10500);
	while(Motor_z.getMotionStatus()!=0){
		usleep(10000);	
	}

	Motor_p.drive(-100);
	for(i=0;i<=10;i++){
		print_x_slow(3000,10);
		print_z_slow(2000,-10);
		print_x_slow(3000,-10);
		print_z_slow(2000,10);
		Motor_z.drive(1000);
		usleep(1000*100);
	}

	Motor_z.drive(10500);
	while(Motor_z.getMotionStatus()!=0){
		usleep(10000);	
	}
	return 1;
}
