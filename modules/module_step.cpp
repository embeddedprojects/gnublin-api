#include "module_step.h"
#include "../include/includes.h"
#include "../drivers/i2c.cpp"

//------------------Konstruktor------------------
// set irun and vmax to the default values 

gnublin_module_step::gnublin_module_step()
{
	irun = 15;
	vmax = 8;
}


//-------------get Error Message-------------
// get the last ErrorMessage
// parameters:		NONE
// return:		[const char*]ErrorMessage	Error Message as c-string

const char *gnublin_module_step::getErrorMessage(){
	return ErrorMessage.c_str();
}


//-------------set Address-------------
// set the slave address
// parameters:		[int]Address	i2c slave Address
// return:			NONE

void gnublin_module_step::setAddress(int Address){
	i2c.setAddress(Address);
}

//-------------------set devicefile----------------
// set the i2c device file. default is "/dev/i2c-1"
// parameters:		[string]filename	path to the dev file
// return:			NONE

void gnublin_module_step::setDevicefile(std::string filename){
	i2c.setDevicefile(filename);
}

//-------------------set irun----------------
// set the Irun = newIrun
// parameters:		[unsigned int]newIrun	new Irun Value
// returns:			[int]  1			success
// 					[int] -1			failure

int gnublin_module_step::setIrun(unsigned int newIrun){
	if(newIrun <= 15){
		irun=newIrun;
		return 1;
	}
	else return -1;
}

//-------------------set ihold----------------
// set the ihold = newIhold
// parameters:		[unsigned int]newIhold	new ihold Value
// returns:			[int]  1				success
// 					[int] -1				failure
int gnublin_module_step::setIhold(unsigned int newIhold){
	if(newIhold <= 15){
		ihold=newIhold;
		return 1;
	}
	else return -1;
}

//-------------------set vmax----------------
// set the vmax = newVmax
// parameters:		[unsigned int]newVmax	new vmax Value
// returns:			[int]  1				success
// 					[int] -1				failure
int gnublin_module_step::setVmax(unsigned int newVmax){
	if(newVmax <= 15){
		vmax=newVmax;
		return 1;
	}
	else return -1;
}

//-------------------set vmin----------------
// set the vmin = newVmin
// parameters:		[unsigned int]newVmin	new vmin Value
// returns:			[int]  1				success
// 					[int] -1				failure
int gnublin_module_step::setVmin(unsigned int newVmin){
	if(newVmin <= 15){
		vmin=newVmin;
		return 1;
	}
	else return -1;
}


//-------------------write TMC----------------
// write to TMC222 Chip
// parameters:	[int]num			amount of bytes
//				[char*]TxBuf 		transmit buffer. the data you want to send is stored in it	
// return:		[int]  1			success
// 				[int] -1			failure

int gnublin_module_step::writeTMC(unsigned char *TxBuf, int num){
	if(!i2c.send(TxBuf, num)){
	    return -1;
   	}
	else return 1;
}

//-------------------read TMC----------------
// read from TMC222 Chip
// parameters:	[int]num			amount of bytes
//				[char*]RxBuf		receive buffer. The Data will be stored in it. 
// return:		[int]  1			success
// 				[int] -1			failure

int gnublin_module_step::readTMC(unsigned char *RxBuf, int num){
   	if(!i2c.receive(RxBuf, num)){
       	return -1;
    }
	else return 1;	
}

//-------------------burn new Address----------------
// burn a new i2c slave address into the TMC222 Chip
// parameters:	[int]new_address	the new slave address, which will be burned into the TMC222 Chip
//
// return:		[int]  1			success
// 				[int] -1			failure

int gnublin_module_step::burnNewAddress(int new_address){
	
	//SetOTPParam
	unsigned char buffer[5];
	int new_ad = 0;
	int old_ad = 0;
	int slave_address=i2c.getAddress();
	std::string yes = "NO!";

 	if(new_address <= slave_address){
		printf("\tThe new address must be higher than the old one (0x%x)!\n",slave_address);
		return -1;
 	}
 	else if (new_address > 0x7f){
		printf("The biggest slave address of the TMC222 is 0x7f\n");
		return -1;
 	}
 	else{
  		old_ad = (slave_address & 0x1e) >> 1;
  		new_ad = (new_address & 0x1e) >> 1;
  		if(((new_ad & 0x1)<(old_ad & 0x1))|((new_ad & 0x2)<(old_ad & 0x2))|((new_ad & 0x4)<(old_ad & 0x4))|((new_ad & 0x8)<(old_ad & 0x8))){
        		printf("\tThis address could not be set, because the '1' cant be undone!\n"
        			"\told OTP AD: 0x%x\n"
        			"\tnew OTP AD: 0x%x\n",old_ad, new_ad);
        		return -1;
  		}
	  	if((new_address & 0x01) == 1){
			printf("\tThe LSB address bit is set by the jumper on the module-step\n");
			new_address --;
			printf("\tThe new address will be set to: 0x%x \n", new_address);
	  	}

	    	printf("\tIf a bit of the OTP (on time programmable) is set, it cant be undone! \n\t"
	    		"If you are sure to write the new Address (0x%x) then type 'yes' in CAPITALS\n\n\t", new_address);
	    	std::cin >> yes;
	    	if(yes == "YES"){
			buffer[0] = 0x90; //SetOTPParam
			buffer[1] = 0xff; //N/A
			buffer[2] = 0xff; //N/A
			buffer[3] = 0x02; //set AD3 AD2 AD1 AD0
			buffer[4] = (unsigned char) new_ad;

		   	if(!i2c.send(buffer, 5)){
			   	return -1;
			}
			else {
				printf("\tNew Address was successfully set to: 0x%x\n\tPlease replug the module.\n\n", new_address);
				return 1;
			}
	    	}
	  	else{
			printf("\tYou didn't type 'YES'\n");
			return -1;
	  	}
	}
}

//-------------------get Full Status 1----------------
// get full status 1 from the TMC222
// parameters:	NONE
//
// return:		[int]  1			success
// 				[int] -1			failure

int gnublin_module_step::getFullStatus1(){
      	if(i2c.send(0x81)){
		return 1;		
		}
		else return -1;
}

//-------------------get Full Status 2----------------
// get full status 2 from the TMC222
// parameters:	NONE
//
// return:		[int]  1			success
// 				[int] -1			failure


int gnublin_module_step::getFullStatus2(){
		if(i2c.send(0xfc)){
			return 1;
		}
		else return -1;
}


//-------------------run Init----------------
// send Run Init command to the TMC222
// parameters:	NONE
//
// return:		[int]  1			success
// 				[int] -1			failure


int gnublin_module_step::runInit(){
		if(i2c.send(0x88)){
		return 1;
		}
		else return -1;
}


//-------------------set Motor Parameter----------------
// set the Motor parameters
// parameters:	[unsigned int]newIrun		new irun value
//				[unsigned int]newIhold		new ihold value
//				[unsigned int]newVmax		new vmax value
//				[unsigned int]newVmin		new vmin value
//
// return:		[int]  1				success
// 				[int] -1				failure


int gnublin_module_step::setMotorParam(){
	unsigned char buffer[8];
	//SetMotorParam
	buffer[0] = 0x89; //SetMotorParam
	buffer[1] = 0xff; //N/A
	buffer[2] = 0xff; //N/A
	buffer[3] = (unsigned char) ((irun * 0x10) + ihold); //Irun & I hold
	buffer[4] = (unsigned char) ((vmax * 0x10) + vmin); //Vmax & Vmin 
	buffer[5] = 0x00; //status
	buffer[6] = 0x00; //securePos
	buffer[7] = 0x00; //StepMode

    if(i2c.send(buffer, 8)){
	return 1;
	}
	else return -1;
}

int gnublin_module_step::setMotorParam(unsigned int newIrun, unsigned int newIhold, unsigned int newVmax, unsigned int newVmin){
	irun=newIrun;
	ihold=newIhold;
	vmax=newVmax;
	vmin=newVmin;

	unsigned char buffer[8];
	//SetMotorParam
	buffer[0] = 0x89; //SetMotorParam
	buffer[1] = 0xff; //N/A
	buffer[2] = 0xff; //N/A
	buffer[3] = (unsigned char) ((irun * 0x10) + ihold); //Irun & I hold
	buffer[4] = (unsigned char) ((vmax * 0x10) + vmin); //Vmax & Vmin 
	buffer[5] = 0x00; //status
	buffer[6] = 0x00; //securePos
	buffer[7] = 0x00; //StepMode

    if(i2c.send(buffer, 8)){
	return 1;
	}
	else return -1;
}

//-------------------hard stop----------------
// send hard stop command to the TMC222
// parameters:	NONE
//
// return:		[int]  1			success
// 				[int] -1			failure

int gnublin_module_step::hardStop(){
		if(i2c.send(0x85)){
		return 1;
		}
		else return -1;
}


//-------------------soft stop----------------
// send soft stop command to the TMC222
// parameters:	NONE
//
// return:		[int]  1			success
// 				[int] -1			failure

int gnublin_module_step::softStop(){
		if(i2c.send(0x8f)){
		return 1;
		}
		else return -1;
}

//-------------------reset position----------------
// send reset Position command to the TMC222 and sets the postion to 0
// parameters:	NONE
//
// return:		[int]  1			success
// 				[int] -1			failure

int gnublin_module_step::resetPosition(){
		if(i2c.send(0x86)){
		return 1;
		}
		else return -1;
}

//-------------------set position----------------
// send set Position command to the TMC222 
// parameters:	[int]position		the new position
//
// return:		[int]  1			success
// 				[int] -1			failure

int gnublin_module_step::setPosition(int position){
	unsigned char buffer[5];
	buffer[0] = 0x8B;   // SetPosition Command
	buffer[1] = 0xff;   // not avialable
	buffer[2] = 0xff;   // not avialable
	buffer[3] = (unsigned char) (position >> 8);  // PositionByte1 (15:8)
	buffer[4] = (unsigned char)  position;       // PositionByte2 (7:0)
	
	if(i2c.send(buffer, 5)){
		return 1;
	}
	else return -1;
}

//-------------------drive----------------
// send set Position command to the TMC222 
// parameters:	[int]steps		the amount of steps to drive (< 0 in negative direction, > 0 in positiv direction)
//
// return:		[int]  1			success
// 			[int] -1			failure
//

int gnublin_module_step::drive(int steps){
	int old_position;
	int new_position;

	old_position = getActualPosition();
	new_position = old_position+steps;
	if(setPosition(new_position)){
		return 1;
	}
	else return -1;
}

//-------------------get Motion Status----------------
// Indicates the actual behavior of the position controller. 
// parameters:	NONE
//
//return:[int]
//	0: Actual Position = Target Position; Velocity= 0
//	1: Positive Acceleration; Velocity > 0
//	2: Negative Acceleration; Velocity > 0
//	3: Acceleration = 0 Velocity = maximum pos Velocity
//	4: Actual Position /= Target Position; Velocity= 0
//	5: Positive Acceleration; Velocity < 0
//	6: Positive Acceleration; Velocity < 0
//	7: Acceleration = 0 Velocity = maximum neg Velocity 
// 
//	-1			failure

int gnublin_module_step::getMotionStatus(){
	unsigned char RxBuf[8];
	int motionStatus = -1;
	getFullStatus1();
	
    	if(!i2c.receive(RxBuf, 8))
		return -1;
	motionStatus = (RxBuf[5] & 0xe0) >> 5;
	return motionStatus;
	
}


//-------------------get Switch status----------------
// check the Switch Status
// parameters:	NONE
//
// return:		[int]  1			Switch closed
//				[int]  0			Switch open
// 				[int] -1			failure

int gnublin_module_step::getSwitch(){
	unsigned char RxBuf[8];    	
	int swi = 0;

    	getFullStatus1();

    	if(i2c.receive(RxBuf, 8)){
	
			if(RxBuf[5] & 0x10){
				swi = 1;				
			}
			else{
				swi = 0;
			}
			return swi;
		}	
		else return -1;
}

//-------------------get actual position----------------
// gets the actual position
// parameters:	NONE
//
// return:		[int]actualPosition		the actual position
// 				[int] -1				failure

int gnublin_module_step::getActualPosition(){
	unsigned char RxBuf[8];	
	int actualPosition=-1;
	
	if(getFullStatus2()==-1)
		return -1;
	
	if(i2c.receive(RxBuf, 8)){
		actualPosition = (RxBuf[1]<<8 | RxBuf[2]);
	}	
	return actualPosition;
}

