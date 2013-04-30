#include "module_pca9555.h"

//*******************************************************************
//Class for accessing GNUBLIN Module-Portexpander or any PCA9555
//*******************************************************************

//-------------Konstruktor-------------
// set error flag=false

gnublin_module_pca9555::gnublin_module_pca9555() 
{
	error_flag=false;
}


//-------------get Error Message-------------
// get the last ErrorMessage
// parameters:		NONE
// return:			[const char*]ErrorMessage	Error Message as c-string

const char *gnublin_module_pca9555::getErrorMessage(){
	return ErrorMessage.c_str();
}

//-------------------------------Fail-------------------------------
//returns the error flag. if something went wrong, the flag is true
bool gnublin_module_pca9555::fail(){
	return error_flag;
}

//-------------set Address-------------
// set the slave address
// parameters:		[int]Address	i2c slave Address
// return:			NONE

void gnublin_module_pca9555::setAddress(int Address){
	i2c.setAddress(Address);
}


//-------------------set devicefile----------------
// set the i2c device file. default is "/dev/i2c-1"
// parameters:		[string]filename	path to the dev file
// return:			NONE

void gnublin_module_pca9555::setDevicefile(std::string filename){
	i2c.setDevicefile(filename);
}

//-------------------Pin Mode----------------
// Controls the Pin Mode.
// parameters:		[int]pin 		Number of the pin
//					[string]		direction OUTPUT/INPUT		
// returns:			[int]  1		success
// 					[int] -1		failure
int gnublin_module_pca9555::pinMode(int pin, std::string direction){
	error_flag=false;
	unsigned char TxBuf[1];
	unsigned char RxBuf[1];

	if (pin < 0 || pin > 15){
		error_flag=true;
		ErrorMessage="Pin Number is not between 0-15";
		return -1;
	}

	if(pin >= 0 && pin <= 7){ // Port 0

			TxBuf[0]=pow(2, pin); //convert pin into its binary form e. g. Pin 3 = 8

			if (i2c.receive(0x06, RxBuf, 1)>0){ //read the current state

				if (direction=="out"){
					TxBuf[0]=RxBuf[0] & ~TxBuf[0]; // at output you have to invert the pin you want to set und AND it to change only the pin 
					if(i2c.send(0x06, TxBuf, 1)>0){
					return 1;
					}
					else {
						error_flag=true;
						ErrorMessage="i2c.send Error";
						return -1;
					}
									
				}
				else if (direction=="in"){
					TxBuf[0]=RxBuf[0] | TxBuf[0]; // at input you just have to do a OR
					if(i2c.send(0x06, TxBuf, 1)>0){
					return 1;
					}
					else {
						error_flag=true;
						ErrorMessage="i2c.send Error";
						return -1;
					}
				}
				else{
					error_flag=true;
					ErrorMessage="direction != IN/OUTPUT";				
					return -1;			
				}
			}
			else {
			error_flag=true;
			ErrorMessage="i2c.receive Error";
			return -1;
			}
	}
	else if(pin >= 8 && pin <= 15){ // Port 1

			TxBuf[0]=pow(2, (pin-8)); //convert pin into its binary form e. g. Pin 3 = 8

			if(i2c.receive(0x07, RxBuf, 1)>0){ //read the current state
			
				if (direction=="out"){
					TxBuf[0]=RxBuf[0] & ~TxBuf[0];
					if(i2c.send(0x07, TxBuf, 1)>0){
					return 1;
					}
					else {
						error_flag=true;
						ErrorMessage="i2c.send Error";
						return -1;
					}					
				}
				else if (direction=="in"){
					TxBuf[0]=RxBuf[0] | TxBuf[0];
					if(i2c.send(0x07, TxBuf, 1)>0){
					return 1;
					}
					else {
						error_flag=true;
						ErrorMessage="i2c.send Error";
						return -1;
					}
				}
				else{
					error_flag=true;
					ErrorMessage="direction != IN/OUTPUT";				
					return -1;			
				}
			}
			else {
			error_flag=true;
			ErrorMessage="i2c.receive Error";
			return -1;
			}
	}
	else return -1;
}


//-------------------Port Mode----------------
// Controls the Port Mode.
// parameters:		[int]port 		Number of the pin
//					[string]		direction OUTPUT/INPUT		
// returns:			[int]  1		success
// 					[int] -1		failure

int gnublin_module_pca9555::portMode(int port, std::string direction){
	error_flag=false;
	unsigned char TxBuf[1];

	if (port < 0 || port > 1){
		error_flag=true;
		ErrorMessage="Port Number is not between 0-1";
		return -1;
	}

	if(port == 0){ // Port 0

			if (direction=="out"){
				TxBuf[0]=0x00;
				if(i2c.send(0x06, TxBuf, 1)>0){
				return 1;
				}
				else {
					error_flag=true;
					ErrorMessage="i2c.send Error";
					return -1;
				}
								
			}
			else if (direction=="in"){
				TxBuf[0]=0xff;
				if(i2c.send(0x06, TxBuf, 1)>0){
				return 1;
				}
				else {
					error_flag=true;
					ErrorMessage="i2c.send Error";
					return -1;
				}
			}
			else{
				error_flag=true;
				ErrorMessage="direction != IN/OUTPUT";				
				return -1;			
			}
	}
	else if(port==1){ // Port 1
		
			if (direction=="out"){
				TxBuf[0]=0x00;
				if(i2c.send(0x07, TxBuf, 1)>0){
				return 1;
				}
				else {
					error_flag=true;
					ErrorMessage="i2c.send Error";
					return -1;
				}					
			}
			else if (direction=="in"){
				TxBuf[0]=0xff;
				if(i2c.send(0x07, TxBuf, 1)>0){
				return 1;
				}
				else {
					error_flag=true;
					ErrorMessage="i2c.send Error";
					return -1;
				}
			}
			else{
				error_flag=true;
				ErrorMessage="direction != IN/OUTPUT";				
				return -1;			
			}
	}
	else return -1;
}

//-------------------digital write----------------
// writes a high or low to the Output
// parameters:		[int]pin 		Number of the pin
//					[int]value		High/Low		
// returns:			[int]  1		success
// 					[int] -1		failure

int gnublin_module_pca9555::digitalWrite(int pin, int value){
	error_flag=false;
	unsigned char TxBuf[1];
	unsigned char RxBuf[1];

	if (pin < 0 || pin > 15){
		error_flag=true;
		ErrorMessage="Pin Number is not between 0-15";
		return -1;
	}


	if(pin >= 0 && pin <= 7){ // Port 0

			TxBuf[0]=pow(2, pin); //convert pin into its binary form e. g. Pin 3 = 8

			if (i2c.receive(0x02, RxBuf, 1)>0){ //read the current state

				if (value==0){
					TxBuf[0]=RxBuf[0] & ~TxBuf[0]; // at low you have to invert the pin you want to set and do a AND to change only the pin you want 
					if(i2c.send(0x02, TxBuf, 1)>0){
					return 1;
					}
					else {
						error_flag=true;
						ErrorMessage="i2c.send Error";
						return -1;
					}
									
				}
				else if (value==1){
					TxBuf[0]=RxBuf[0] | TxBuf[0]; // at high you just have to do a OR
					if(i2c.send(0x02, TxBuf, 1)>0){
					return 1;
					}
					else {
						error_flag=true;
						ErrorMessage="i2c.send Error";
						return -1;
					}
				}
				else{
					error_flag=true;
					ErrorMessage="value != 1/0";				
					return -1;			
				}
			}
			else {
			error_flag=true;
			ErrorMessage="i2c.receive Error";
			return -1;
			}
	}
	else if(pin >= 8 && pin <= 15){ // Port 1

			TxBuf[0]=pow(2, (pin-8)); //convert pin into its binary form e. g. Pin 3 = 8

			if(i2c.receive(0x03, RxBuf, 1)>0){ //read the current state
				if (value==0){
					
					TxBuf[0]=RxBuf[0] & ~TxBuf[0];
					if(i2c.send(0x03, TxBuf, 1)>0){
					return 1;
					}
					else {
						error_flag=true;
						ErrorMessage="i2c.send Error";
						return -1;
					}					
				}
				else if (value==1){
					TxBuf[0]=RxBuf[0] | TxBuf[0];
					if(i2c.send(0x03, TxBuf, 1)>0){
					return 1;
					}
					else {
						error_flag=true;
						ErrorMessage="i2c.send Error";
						return -1;
					}
				}
				else{
					error_flag=true;
					ErrorMessage="value != HIGH/LOW";				
					return -1;			
				}
			}
			else {
			error_flag=true;
			ErrorMessage="i2c.receive Error";
			return -1;
			}
	}
	else return -1;
}

//-------------------write port----------------
// Writes one byte to a complete port
// parameters:	[int]port 0/1 		Number of the port
//				[unsigned char]value	Byte to write
//							
// returns:		[int]  1		succsess
// 				[int] -1		failure

int gnublin_module_pca9555::writePort(int port, unsigned char value){
	error_flag=false;
	unsigned char buffer[1];
	buffer[0]=value;

	if(port==0){ // Port 0
		if(i2c.send(0x02, buffer, 1)>0){
			return 1;
		}
		else {
			error_flag=true;
			ErrorMessage="i2c.send Error";
			return -1;
		}
	}
	else if(port==1){
		if(i2c.send(0x03, buffer, 1)>0){
			return 1;
		}
		else {
			error_flag=true;
			ErrorMessage="i2c.send Error";
			return -1;
		}
	}		
	else{
		error_flag=true;
		ErrorMessage="Pin Number is not between 0-1";
		return -1;		
	}
	
}

//-------------------digital read----------------
// reads the state of the inputs and returns it
// parameters:		[int]pin 		Number of the pin
//							
// returns:			[int]  0/1		logical level of the pin
// 					[int] -1		failure

int gnublin_module_pca9555::digitalRead(int pin) {
	error_flag=false;
	unsigned char RxBuf[1];

	if (pin < 0 || pin > 15){
		error_flag=true;
		ErrorMessage="Pin Number is not between 0-15\n";
		return -1;
	}
	
	if(pin >= 0 && pin <= 7){ // Port 0		
		if(i2c.receive(0x00, RxBuf, 1)>0){

				RxBuf[0]<<=(7-pin); // MSB is now the pin you want to read from
				RxBuf[0]&=128; // set all bits to 0 except the MSB
		
				if(RxBuf[0]==0){
					return 0;
				}
				else if (RxBuf[0]==128){
					return 1;
				}
				else{
					error_flag=true;
					ErrorMessage="bitshift failed\n";
					return -1;
				}
		}
		else{
			error_flag=true;
			ErrorMessage="i2c.receive Error";
			return -1;
		}
	}
	else if(pin >= 8 && pin <= 15){ // Port 1
		if(i2c.receive(0x01, RxBuf, 1)>0){

				RxBuf[0]<<=(15-pin); // MSB is now the pin you want to read from
				RxBuf[0]&=128;	// set all bits to 0 except the MSB	
		
				if(RxBuf[0]==0){
					return 0;
				}
				else if (RxBuf[0]==128){
					return 1;
				}
				else{
					error_flag=true;
					ErrorMessage="bitshift failed\n";
					return -1;
				}
		}
		else{
			error_flag=true;
			ErrorMessage="i2c.receive Error";
			return -1;
		}
	}
	error_flag=true;
	ErrorMessage="something went wrong";
	return -1;
}
