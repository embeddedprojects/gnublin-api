#include "module_pca9555.h"

//*******************************************************************
//Class for accessing GNUBLIN Module-Portexpander or any PCA9555
//*******************************************************************

//------------------Konstruktor------------------
/** @~english 
* @brief Set standard i2c address 0x20, set ErrorFlag false, sets the closemode to "1" (see i2c for details)
*
* @~german 
* @brief Setze standard i2c Adresse 0x20, den closemode auf "1" (siehe i2c für Details) und setze das ErrorFlag auf false.
*
*/
gnublin_module_pca9555::gnublin_module_pca9555() 
{
	error_flag=false;
	setAddress(0x20);
}


//-------------get Error Message-------------
/** @~english 
* @brief Get the last Error Message.
*
* This function returns the last Error Message, which occurred in that Class.
* @return ErrorMessage as c-string
*
* @~german 
* @brief Gibt die letzte Error Nachricht zurück.
*
* Diese Funktion gibt die Letzte Error Nachricht zurück, welche in dieser Klasse gespeichert wurde.
* @return ErrorMessage als c-string
*/
const char *gnublin_module_pca9555::getErrorMessage(){
	return ErrorMessage.c_str();
}

//-------------------------------Fail-------------------------------
/** @~english 
* @brief returns the error flag to check if the last operation went wrong
*
* @return error_flag as boolean
*
* @~german 
* @brief Gibt das error_flag zurück um zu überprüfen ob die vorangegangene Operation einen Fehler auweist
*
* @return error_flag als bool
*/
bool gnublin_module_pca9555::fail(){
	return error_flag;
}

//-------------set Address-------------
/** @~english 
* @brief Set the i2c slave address 
*
* With this function you can set the individual I2C Slave-Address of the PCA9555.
* @param Address new I2C slave Address
*
* @~german 
* @brief Setzt die i2c slave Adresse
*
* Mit dieser Funktion kann die individuelle I2C Slave-Adresse des PCA9555 gesetzt werden.
* @param Address neue I2C slave Adresse
*/
void gnublin_module_pca9555::setAddress(int Address){
	i2c.setAddress(Address);
}


//-------------------set devicefile----------------
/** @~english
* @brief set i2c the device file. default is "/dev/i2c-1"
*
* This function sets the devicefile you want to access. by default "/dev/i2c-1" is set.
* @param filename path to the devicefile e.g. "/dev/i2c-0"
*
* @~german
* @brief setzt die I2C Device Datei. Standard ist die "/dev/i2c-1"
*
* Diese Funktion setzt die Geräte Datei, auf die man zugreifen möchte. Standardmäßig ist bereits "/dev/i2c-1" gesetzt.
* @param filename Dateipfad zur Geräte Datei, z.B. "/dev/i2c-0"
*/
void gnublin_module_pca9555::setDevicefile(std::string filename){
	i2c.setDevicefile(filename);
}

//-----------------------------------Pin Mode-----------------------------------
/** @~english
* @brief Controls the pin mode (INPUT/OUTPUT)
*
* With this Function you can set a single pin of the PCA9555 wether as input or output.
* @param pin Number of the pin (0-15)
* @param direction INPUT or OUTPUT
* @return success: 1, failure: -1
*
* @~german
* @brief Setzt den Pin Modus (Eingang/Ausgang)
*
* Mit dieser Funktion kann man einen einzelnen Pin entweder als Eingang oder als Ausgang setzen.
* @param pin Nummer des Pins (0-15)
* @param direction INPUT (Eingang) oder OUTPUT (Ausgang)
* @return Erfolg: 1, Misserfolg: -1
*/
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


//-----------------------------------Port Mode-----------------------------------
/** @~english
* @brief Controls the port mode (INPUT/OUTPUT)
*
* With this Function you can set a whole port of the PCA9555 wether as input or output.
* @param port Number of the port (0-1)
* @param direction INPUT or OUTPUT
* @return success: 1, failure: -1
*
* @~german
* @brief Setzt den Port Modus (Eingang/Ausgang)
*
* Mit dieser Funktion kann man einen ganzen Port entweder als Eingang oder als Ausgang setzen.
* @param port Nummer des Ports (0-1)
* @param direction INPUT (Eingang) oder OUTPUT (Ausgang)
* @return Erfolg: 1, Misserfolg: -1
*/
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


//-----------------------------------digital Write-----------------------------------
/** @~english
* @brief sets the level of an output pin (HIGH/LOW)
*
* With this Function you can set the logical level of an output pin to wether HIGH or LOW.
* @param pin Number of the pin (0-15)
* @param value HIGH (1) or LOW (0)
* @return success: 1, failure: -1
*
* @~german
* @brief Setzt den Pegel eines Ausgangs (HIGH/LOW)
*
* Mit dieser Funktion kann man den Pegel eines Ausgangs entweder auf HIGH oder auf LOW setzen.
* @param pin Nummer des Pins (0-15)
* @param value HIGH (1) oder LOW (0) 
* @return Erfolg: 1, Misserfolg: -1
*/
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
					TxBuf[0]=(RxBuf[0] & ~TxBuf[0]); // at low you have to invert the pin you want to set and do a AND to change only the pin you want 
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
					TxBuf[0]=(RxBuf[0] | TxBuf[0]); // at high you just have to do a OR
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

//-----------------------------------write Port-----------------------------------
/** @~english
* @brief  Writes one byte to a complete port
*
* With this Function you can write a whole byte to a whole port of the PCA9555.
* @param port Number of the port (0-1)
* @param value The Byte you want to write.  e.g. 0x12 to write 00010010 
* @return success: 1, failure: -1
*
* @~german
* @brief Schreibt ein Byte an einen kompletten Port
*
* Mit dieser Funktion kann man ein komplettes Byte an einen ganzen Port des PCA9555 schreiben.
* @param port Nummer des Ports (0-1)
* @param value Das Byte, dass man schreiben möchte. z.B. 0x12 um 00010010 zu schreiben. 
* @return Erfolg: 1, Misserfolg: -1
*/
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

//-----------------------------------digital read-----------------------------------
/** @~english
* @brief reads the state of an input pin and returns it
*
* With this function you can read the level of an input pin.
* @param pin Number of the pin (0-15) you want to read from
* @return 0/1 logical level of the pin, failure: -1
*
* @~german
* @brief Liest den Zustand eins Eingang-Pins und gibt ihn zurück
*
* Mit dieser Funktion kann man den Zustand eines Pins auslesen.
* @param pin Nummer des Pins (0-15) von dem man lesen will.
* @return 0/1 logischer Pegel des Pins, Misserfolg: -1
*/
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

//-----------------------------------read Port-----------------------------------
/** @~english
* @brief reads the state of an input port and returns it
*
* With this function you can read the levels of a whole input port.
* @param port Number of the port (0-1) you want to read from
* @return unsigned char containing the values, failure: -1
*
* @~german
* @brief Liest den Zustand eins Eingang-Ports und gibt ihn zurück
*
* Mit dieser Funktion kann man den Zustand eines Ports auslesen.
* @param port Nummer des Ports (0-1) von dem man lesen will.
* @return unsigned char mit den Pegeln des Ports, Misserfolg: -1
*/
unsigned char gnublin_module_pca9555::readPort(int port) {
	error_flag=false;
	unsigned char RxBuf[1];

	if(port == 0 ){ // Port 0		
		if(i2c.receive(0x00, RxBuf, 1)>0){

			return RxBuf[0];
				
		}
		else{
			error_flag=true;
			ErrorMessage="i2c.receive Error";
			return -1;
		}
	}
	else if(port == 1){ // Port 1
		if(i2c.receive(0x01, RxBuf, 1)>0){

			return RxBuf[0];
				
		}
		else{
			error_flag=true;
			ErrorMessage="i2c.receive Error";
			return -1;
		}
	}
	else {
		error_flag=true;
		ErrorMessage="Port Number is not between 0-1\n";
		return -1;
	}
}


//-----------------------------------readState-----------------------------------
/** @~english
* @brief reads the state of a OUTPUT pin and returns it
*
* With this function you can read the level of a pin.
* @param pin Number of the pin (0-15) you want to read from
* @return 0/1 logical level of the pin, failure: -1
*
* @~german
* @brief Liest den Zustand eines OUTPUT Pins und gibt ihn zurück
*
* Mit dieser Funktion kann man den Zustand eines Pins auslesen.
* @param pin Nummer des Pins (0-15) von dem man lesen will.
* @return 0/1 logischer Pegel des Pins, Misserfolg: -1
*/
int gnublin_module_pca9555::readState(int pin) {
  error_flag=false;
  unsigned char RxBuf[1];

  if (pin < 0 || pin > 15){
    error_flag=true;
    ErrorMessage="Pin Number is not between 0-15\n";
    return -1;
  }

  if(pin >= 0 && pin <= 7){ // Port 0   
    if(i2c.receive(0x02, RxBuf, 1)>0){

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
		if(i2c.receive(0x03, RxBuf, 1)>0){

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

