#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <unistd.h>
#include <getopt.h>
#include <string>
#include "../gnublin-api/drivers/i2c.cpp"
//#include "../gnublin-api/modules/adc.cpp"



#define ADDR 0x60

using namespace std;

int newPosition = 0;
int actualPosition = -1;
int c,hflag;
int slave_address = ADDR;
char buffer[128];
char rx_buf[128];
int json_flag = 0;
int brute_flag = 0;
unsigned int irun = 15;
unsigned int ihold = 0;
unsigned int vmax = 8;
unsigned int vmin = 0;
int new_address = -1;
int overCurrent = 0;


gnublin_i2c i2c;
//gnublin_adc adc;




int writeTMC(int num){
	if(!i2c.send(buffer, num)){
	      	if (json_flag == 1)
		        printf("{\"error_msg\" : \"write error/no module is connected on address 0x%x\",\"result\" : \"-4\"}\n",slave_address);
	     	else
		       	printf("ERROR: Write Error/No module is connected on address 0x%x\n",slave_address);
			return -1;
      	}
}

int readTMC(int num){
	int i = 0;
    	for (i; i < num; i++){
       		rx_buf[i] = 0;
    	}
    	i = 0;
    	if(!i2c.receive(rx_buf, num)){
        	if (json_flag == 1)
		        printf("{\"error_msg\" : \"read error/no module is connected on address 0x%x\",\"result\" : \"-4\"}\n",slave_address);
	     	else
		       	printf("ERROR: Read Error/No module is connected on address 0x%x\n",slave_address);
			return -1;
        }
}

int newAddress(){

	//SetOTPParam
	int new_ad = 0;
	int old_ad = 0;
	string yes = "NO!";

 	if(new_address <= slave_address){
		printf("\tThe new address must be higher than the old one (0x%x)!\n",slave_address);
		exit(1);
 	}
 	else if (new_address > 0x7f){
		printf("The biggest slave address of the TMC222 is 0x7f\n");
		exit(1);
 	}
 	else{
  		old_ad = (slave_address & 0b0011110) >> 1;
  		new_ad = (new_address & 0b0011110) >> 1;
  		if((new_ad & 0b0001)<(old_ad & 0b0001)|(new_ad & 0b0010)<(old_ad & 0b0010)|(new_ad & 0b0100)<(old_ad & 0b0100)|(new_ad & 0b1000)<(old_ad & 0b1000)){
        		printf("\tThis address could not be set, because the '1' cant be undone!\n"
        			"\told OTP AD: 0x%x\n"
        			"\tnew OTP AD: 0x%x\n",old_ad, new_ad);
        		exit(1);
  		}
	  	if((new_address & 0b00000001) == 1){
			printf("\tThe LSB address bit is set by the jumper on the module-step\n");
			new_address --;
			printf("\tThe new address will be set to: 0x%x \n", new_address);
	  	}

	    	printf("\tIf a bit of the OTP (on time programmable) is set, it cant be undone! \n\t"
	    		"If you are sure to write the new Address (0x%x) then type 'yes' in CAPITALS\n\n\t", new_address);
	    	cin >> yes;
	    	if(yes == "YES"){
			buffer[0] = 0x90; //SetOTPParam
			buffer[1] = 0xff; //N/A
			buffer[2] = 0xff; //N/A
			buffer[3] = 0x02; //set AD3 AD2 AD1 AD0
			buffer[4] = (unsigned char) new_ad;

			writeTMC(5);

			printf("\tNew Address was successfully set to: 0x%x\n\tPlease replug the module.\n\n", new_address);
			exit(1);
	    	}
	  	else{
			printf("\tYou didn't type 'YES'\n");
			exit(1);
	  	}
	}
}

int getFullStatus1(){
      	buffer[0] = 0x81;
      	writeTMC(1);
}

int getFullStatus2(){
      	buffer[0] = 0xfc;
      	writeTMC(1);
}

int runInit(){
      	buffer[0] = 0x88;
	writeTMC(1);
}

int setMotorParam(){
	//SetMotorParam
	buffer[0] = 0x89; //SetMotorParam
	buffer[1] = 0xff; //N/A
	buffer[2] = 0xff; //N/A
	buffer[3] = (unsigned char) ((irun * 0x10) + ihold); //Irun & I hold
	buffer[4] = (unsigned char) ((vmax * 0x10) + vmin); //Vmax & Vmin 
	buffer[5] = 0x00; //status
	buffer[6] = 0x00; //securePos
	buffer[7] = 0x00; //StepMode

      	writeTMC(8);
}

int hardStop(){
	buffer[0] = 0x85;
	writeTMC(1);
}

int softStop(){
	buffer[0] = 0x8f;
	writeTMC(1);
}

int resetPosition(){
	buffer[0] = 0x86;
	writeTMC(1);
}

int setPosition(int position){
	buffer[0] = 0x8B;   // SetPosition Command
	buffer[1] = 0xff;   // not avialable
	buffer[2] = 0xff;   // not avialable
	buffer[3] = (unsigned char) (position >> 8);  // PositionByte1 (15:8)
	buffer[4] = (unsigned char)  position;       // PositionByte2 (7:0)
	writeTMC(5);
}

int getSwi(){
    	int num=8;
    	int swi = 0;
    	getFullStatus1();
    	readTMC(8);
	
	if(rx_buf[5] & 0x10){
		//printf("\tSchalter gedrückt!\n");
		swi = 1;
	}
	else{
		//printf("\tSchalter NICHT gedrückt!\n");
		swi = 0;
	}
	return swi;
}

int getOverCurrent(){
    	int num=8;
    	int swi = 0;
       	getFullStatus1();
    	readTMC(8);
	
	if((rx_buf[5] & 0x08) | (rx_buf[5] & 0x04)){
		overCurrent = 1;
		cout << "OverCurrent flag set!!" << endl;
	}
	else{
		overCurrent = 0;
	}
	return overCurrent;
}

int getActualPosition(){
	getFullStatus2();
	readTMC(8);
	int i;
	actualPosition = (rx_buf[1]<<8 | rx_buf[2]);
	return actualPosition;
}



void parse_opts(int argc, char **argv)
{

        while((c = getopt(argc,argv,"hjp:f:a:bi:d:x:n:o:r")) != -1)
        {
                switch(c)
                {
                        case 'h' : hflag = 1;                               break;                              /* help */
                        case 'p' : newPosition = atoi(optarg);              break;
                        case 'f' : i2c.setDevicefile(optarg);               break;
                        case 'a' : slave_address = strtol (optarg,NULL,16); break;
                        case 'j' : json_flag = 1;                           break;
                        case 'b' : brute_flag = 1;                          break;
			case 'i' : irun = atoi(optarg);			    break;
			case 'd' : ihold = atoi(optarg);		    break;
			case 'x' : vmax = atoi(optarg);			    break;
			case 'n' : vmin = atoi(optarg);			    break;
			case 'o' : new_address = strtol (optarg,NULL,16);   break;
			case 'r' : resetPosition();			    break;
		}

        }
        if (hflag | argc == 1)
        {
                printf("This program is designed, to easily interact with a stepper-motor connected to the GNUBLIN.\n\n"
                	"-h Show this help\n"
                	"-f <device> Specify the i2c-device.default=/dev/i2c-1\n"
                	"-j Convert output to json format.\n"
                	"-b show output in raw format.\n"
                	"-a <I2C-address> Specify the stepper modules I2C-address.default=0x60\n"
                	"-p <Position> Specify the desired position\n"
                	"-i <Irun> Specify the Irun parameter (0-15)\n"
                	"-d <Ihold> Specify the iholD parameter (0-15)\n"
                	"-x <vmax> Specify the vmaX parameter (0-15)\n-"
                	"n <vmin> Specify the vmiN parameter (0-15)\n"
                	"-o <new_address> burns the new I2C Address to the controller\n\n"
                	"Examples:\n\n"
                	"\tDrive the motor to position 3000 and use I2C-address 0x60:\n"
                	"\t%s -a 0x60 -p 3000\n"
                	"\tA complete rotation is position 3200, two rotations 6400 and so on.\n", argv[0]);  
                	
        	exit(1);
        }
}	

int main (int argc, char **argv) {

    	parse_opts(argc, argv);

        i2c.setAddress(slave_address);

    	if(new_address != -1)
        {
        	newAddress();
		exit(1);
        }

	//SetMotorParam
	setMotorParam();      	
	//GetFullStatus1 command:  This Command must be executed before Operating
      	getFullStatus1();
	//RunInit command:  This Command must be executed before Operating
      	runInit();

	//SetPosition
	getActualPosition();
	setPosition(newPosition + actualPosition);
/*	if(newPosition > 0)
		if((actualPosition + newPosition) <= 11400){
			
		}
		else
			cout <<"\t don't drive through the end! ;)" << endl;
	else{
		if((actualPosition + newPosition) >= 0){
			setPosition(actualPosition + newPosition);
		}
		else
			cout << "\t don't drive through the end! ;)" << endl;
	}
	
*/
	
	cout << "Actual Possition is " << actualPosition + newPosition << endl;
	
}

