#include "i2c.h"

//*******************************************************************
//Class for accessing GNUBLIN i2c Bus
//*******************************************************************

//------------------Konstruktor------------------
/** @~english 
* @brief Sets the error_flag to "false" and the devicefile to "/dev/i2c-1"
*
* @~german 
* @brief Setzt das error_flag auf "false" und das devicefile auf standardmäßig "/dev/i2c-1"
*
*/
gnublin_i2c::gnublin_i2c()
{
	devicefile="/dev/i2c-1";
	error_flag=false;
	close_mode=false;
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
bool gnublin_i2c::fail(){
	return error_flag;
}

//-------------set Address-------------
/** @~english 
* @brief Set the i2c slave address 
*
* With this function you can set the individual I2C Slave-Address.
* @param Address new I2C slave Address
*
* @~german 
* @brief Setzt die i2c slave Adresse
*
* Mit dieser Funktion kann die individuelle I2C Slave-Adresse gesetzt werden.
* @param Address neue I2C slave Adresse
*/
void gnublin_i2c::setAddress(int Address){
	slave_address = Address;
}

//-------------get Address-------------
/** @~english 
* @brief Get the i2c slave address 
*
* With this function you can get the set Slave-Address.
* @return Address I2C slave Address
*
* @~german 
* @brief gibt die gesetzte I2C Slave Adresse zurück
*
* Mit dieser Funktion kann die gesetzte I2C Slave-Adresse ausgelesen werden.
* @return Address I2C slave Address
*/
int gnublin_i2c::getAddress(){
	return slave_address;
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
const char *gnublin_i2c::getErrorMessage(){
	return ErrorMessage.c_str();
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
void gnublin_i2c::setDevicefile(std::string filename){
	devicefile = filename;
}


//-------------------open devicefile----------------


int gnublin_i2c::openDevFile(){
	if ((fd = open(devicefile.c_str(), O_RDWR)) < 0) {
			ErrorMessage="ERROR opening: " + devicefile + "\n";
			error_flag=true;
			close(fd);    	
			return -1;
	}
	return 1;
}

//-------------------close devicefile----------------


void gnublin_i2c::closeDevFile(){
	close(fd);
}

//-------------------set closemode-------------------
void gnublin_i2c::setclosemode(int mode){
	if(mode>=1){
	close_mode=true;
	}
	else if (mode==0){
	close_mode=false;
	}
}

//----------------------------------receive----------------------------------
/** @~english 
* @brief receive bytes from the I2C bus.
*
* This function reads "length" number of bytes from the i2c bus and stores them into the "RxBuf". At success the function returns 1, on failure -1.<br>
* e.g.<br>
* read 2 bytes into buf<br>
* receive(buf, 2);
* @param RxBuf Receive buffer. The read bytes will be stored in it.
* @param length Amount of bytes that will be read.
* @return success: 1, failure: -1
*
* @~german 
* @brief Empfängt Bytes vom I2C Bus.
*
* Diese Funktion liest "length" Anzahl an Bytes vom I2C Bus und speichert diese in "RxBuf". Bei Erfolg wird 1 zurück gegeben, bei Misserfolg -1.<br>
* Beispiele:<br>
* Lese 2 Bytes und speichere diese in "buf":<br>
* receive(buf, 2);
* @param RxBuf Empfangs Puffer. Die gelesenen Bytes werden hier gespeichert.
* @param length Anzahl der zu lesenden Bytes.
* @return Erfolg: 1, Misserfolg: -1
*/
int gnublin_i2c::receive(unsigned char *RxBuf, int length){
	error_flag=false;
	
	if(close_mode==true){
		if(openDevFile()<0){
			return -1;
		}
	}

	if (ioctl(fd, I2C_SLAVE, slave_address) < 0) {
		ErrorMessage="ERROR address: " + numberToString(slave_address) + "\n";
		error_flag=true; 
		close(fd);
    	return -1;
  	}

	if (read(fd, RxBuf, length) != length){
		ErrorMessage="i2c read error! Address: " + numberToString(slave_address) + " dev file: " + devicefile + "\n";		
		error_flag=true; 
		close(fd);
		return -1;
	}
	
	if(close_mode==true){
		closeDevFile(); 
	}
	return 1;
}

//----------------------------------receive----------------------------------
/** @~english 
* @brief receive bytes from the I2C bus.
*
* This function reads "length" number of bytes from the register "RegisterAddress" and stores them into the "RxBuf". At success the function returns 1, on failure -1.<br>
* e.g.<br>
* read 2 bytes into buf<br>
* receive(buf, 2);<br><br>
* 
* read 3 bytes into buf from a register with the address 0x12<br>
* receive(0x12, buf, 3);
* @param RegisterAddress Address of the register you want to read from
* @param RxBuf Receive buffer. The read bytes will be stored in it.
* @param length Amount of bytes that will be read.
* @return success: 1, failure: -1
*
* @~german 
* @brief Empfängt Bytes vom I2C Bus.
*
* Diese Funktion liest "length" Anzahl an Bytes aus dem Register "RegisterAddress" und speichert diese in "RxBuf". Bei Erfolg wird 1 zurück gegeben, bei Misserfolg -1.<br>
* Beispiele:<br>
* Lese 2 Bytes und speichere diese in "buf":<br>
* receive(buf, 2);<br><br>
*
* Lese 3 Bytes aus Register 0x12 und speichere sie in "buf":<br>
* receive(0x12, buf, 3);
* @param RegisterAddress Adresse des zu lesenden Registers.
* @param RxBuf Empfangs Puffer. Die gelesenen Bytes werden hier gespeichert.
* @param length Anzahl der zu lesenden Bytes.
* @return Erfolg: 1, Misserfolg: -1
*/
int gnublin_i2c::receive(unsigned char RegisterAddress, unsigned char *RxBuf, int length){
	error_flag=false;	

	if(close_mode==true){
		if(openDevFile()<0){
			return -1;
		}
	}	

	if (ioctl(fd, I2C_SLAVE, slave_address) < 0) {
		ErrorMessage="ERROR address: " + numberToString(slave_address) + "\n";
		error_flag=true; 
		close(fd);
    	return -1;
  	}

	if (write(fd, &RegisterAddress, 1) != 1){
		ErrorMessage="i2c write error!\n";
		error_flag=true; 
		close(fd);
		return -1;
		}


	if (read(fd, RxBuf, length) != length){
		ErrorMessage="i2c read error! Address: " + numberToString(slave_address) + " dev file: " + devicefile + "\n";
		error_flag=true; 
		close(fd);
		return -1;
	}

	if(close_mode==true){
		closeDevFile(); 
	} 	 
	return 1;
}

//----------------------------------send----------------------------------
/** @~english 
* @brief send bytes to the I2C bus.
*
* This function sends "length" number of bytes from the "TxBuf" to the i2c bus. At success the function returns 1, on failure -1.<br>
* e.g.<br>
* send 2 bytes from buf to the I2C bus<br>
* send (buf, 2);
* @param TxBuf Transmit buffer. The bytes you want to send are stored in it.
* @param length Amount of bytes that will be send.
* @return success: 1, failure: -1
*
* @~german 
* @brief sendet Bytes an den I2C Bus.
*
* Diese Funktion sendet "length" Anzahl an Bytes aus dem "TxBuf" an den I2C Bus. Bei Erfolg wird 1 zurück gegeben, bei Misserfolg -1.<br>
* Beispiele:<br>
* Sende 2 Bytes von "buf" an den i2c Bus:
* send(buf, 2);
* @param RxBuf Sende Puffer. Die zu sendenden Bytes sind hier gespeichert.
* @param length Anzahl der zu sendenden Bytes.
* @return Erfolg: 1, Misserfolg: -1
*/
int gnublin_i2c::send(unsigned char *TxBuf, int length){
	error_flag=false;	
	
	if(close_mode==true){
		if(openDevFile()<0){
			return -1;
		}
	}	

	if (ioctl(fd, I2C_SLAVE, slave_address) < 0) {
		ErrorMessage="ERROR address: " + numberToString(slave_address) + "\n";
		error_flag=true; 
		close(fd);
    	return -1;
  	}

	if(write(fd, TxBuf, length) != length){
		ErrorMessage="i2c write error!\n";
		error_flag=true; 
		close(fd);
		return -1;
	}

	if(close_mode==true){
		closeDevFile(); 
	}	
	return 1;
}

//----------------------------------send----------------------------------
/** @~english 
* @brief send bytes to the I2C bus.
*
* This function sends "length" number of bytes from the "TxBuf" to the register "RegisterAddress". At success the function returns 1, on failure -1.<br>
* e.g.<br>
* send 2 bytes from buf to the I2C bus<br>
* send (buf, 2);<br><br>
*
* send 3 bytes from buf to a register with the address 0x12<br>
* send (0x12, buf, 3);
* @param RegisterAddress Address of the register you want to send the bytes to
* @param TxBuf Transmit buffer. The bytes you want to send are stored in it.
* @param length Amount of bytes that will be send.
* @return success: 1, failure: -1
*
* @~german 
* @brief sendet Bytes an den I2C Bus.
*
* Diese Funktion sendet "length" Anzahl an Bytes aus dem "TxBuf" an das Register "RegisterAddress". Bei Erfolg wird 1 zurück gegeben, bei Misserfolg -1.<br>
* Beispiele:<br>
* Sende 2 Bytes von "buf" an den i2c Bus:
* send(buf, 2);<br><br>
*
* Sende 3 Bytes aus "buf" an das Register mit der Adresse 0x12:<br>
* send(0x12, buf, 3);
* @param RegisterAddress Adresse des Registers in das man schreiben will.
* @param RxBuf Sende Puffer. Die zu sendenden Bytes sind hier gespeichert.
* @param length Anzahl der zu sendenden Bytes.
* @return Erfolg: 1, Misserfolg: -1
*/
int gnublin_i2c::send(unsigned char RegisterAddress, unsigned char *TxBuf, int length){
	error_flag=false;	
	int i;
	unsigned char data[length+1];
	data[0]=RegisterAddress;

	for ( i = 0; i < length ; i++ ) {
		data[ i + 1 ] = (char)TxBuf[ i ];
	}

	if(close_mode==true){
		if(openDevFile()<0){
			return -1;
		}
	}

	if (ioctl(fd, I2C_SLAVE, slave_address) < 0) {
		ErrorMessage="ERROR address: " + numberToString(slave_address) + "\n";
		error_flag=true; 
		close(fd);
    	return -1;
  	}
	
	
	if(write(fd, data, length+1) != length+1){
		ErrorMessage="i2c write error!\n";
		error_flag=true; 
		close(fd);
		return -1;
	}

	if(close_mode==true){
		closeDevFile(); 
	}
	return 1;
}

//----------------------------------send----------------------------------
/** @~english 
* @brief send a byte to the I2C bus.
*
* This function sends a byte to the i2c bus. At success the function returns 1, on failure -1.<br>
* e.g.<br>
* send 0xff to the I2C bus<br>
* send (0xff);
* @param value byte that will be send.
* @return success: 1, failure: -1
*
* @~german 
* @brief sendet 1 Byte an den I2C Bus.
*
* Diese Funktion sendet ein byte an den i2c Bus. Bei Erfolg wird 1 zurück gegeben, bei Misserfolg -1.<br>
* Beispiel:<br>
* Sende 0xff an den i2c Bus:<br>
* send(0xff);
* @param value Byte das gesendet wird.
* @return Erfolg: 1, Misserfolg: -1
*/
int gnublin_i2c::send(int value){
	error_flag=false;
	int buffer[1];
	buffer[0]=value;	
	
	if(close_mode==true){
		if(openDevFile()<0){
			return -1;
		}
	}	

	if (ioctl(fd, I2C_SLAVE, slave_address) < 0) {
		ErrorMessage="ERROR address: " + numberToString(slave_address) + "\n";
		error_flag=true; 
		close(fd);
    	return -1;
  	}

	if(write(fd, buffer, 1) != 1){
		ErrorMessage="i2c write error!\n";
		error_flag=true; 
		close(fd);
		return -1;
	}

	if(close_mode==true){
		closeDevFile(); 
	}	
	return 1;
}
