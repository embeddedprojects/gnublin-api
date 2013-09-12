#include "module_dac.h"
//*******************************************************************
//Class for accessing the MCP4728 digital to analog IC via I2C
//*******************************************************************

//------------------Konstruktor------------------
/** @~english 
* @brief Sets the error_flag to "false", the closemode to "1" (see i2c for details) and the standard i2c Address to 0x60
*
* @~german 
* @brief Setzt das error_flag auf "false", den closemode auf "1" (siehe i2c für Details) und die Standard i2c Adresse auf 0x60
*
*/
gnublin_module_dac::gnublin_module_dac()
{
  error_flag=false;
	setAddress(0x60);
	_channel[0] = read(0);
	_channel[1] = read(1);
	_channel[2] = read(2);
	_channel[3] = read(3);
	_gain[0] = _gain[1] = _gain[2] = _gain[3] = 0;
	_vRef[0] = _vRef[1] = _vRef[2] = _vRef[3] = 0;
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

const char *gnublin_module_dac::getErrorMessage(){
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

bool gnublin_module_dac::fail(){
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
void gnublin_module_dac::setAddress(int Address){
	i2c.setAddress(Address);
}

//-------------writeAll---------------------
/** @~english 
* @brief Writes to all channels. No EEPROM access. Fast access.
*
* With this function you can write to all channels. EEPROM is not accessed. Fast access. On the MCP4728 chip its the fast write command.
* @param val_0 value for first channel
* @param val_1 value for second channel
* @param val_2 value for thrid channel
* @param val_3 value for fourth channel
*
*
* @~german 
* @brief Schreibt in alle Kanäle. Kein EEPROM
*
* Mit dieser Funktion kann man in alle Kanäle schreiben. Werte werden nicht ins EEPROM geschrieben. Dadurch ist schneller Zugriff gewährleistet. Auf dem Chip MCP4728 entspricht dies dem fast write command.
* @param val_0 Wert des ersten Kanals
* @param val_1 Wert des zweiten Kanals
* @param val_2 Wert des dritten Kanals
* @param val_3 Wert des vierten Kanals
*
*/

void gnublin_module_dac::writeAll(int val_0, int val_1, int val_2, int val_3) {
	_channel[0] = val_0;
	_channel[1] = val_1;
	_channel[2] = val_2;
	_channel[3] = val_3;
	int i;
	char lowByte[4];
	char highByte[4];

	for (i=0; i<4; i++) {
		lowByte[i] = (char) (_channel[i] & 0xff);
		highByte[i] = (char) ((_channel[i] >> 8) & 0xff);
	}

	unsigned char buffer[8]={	highByte[0],lowByte[0],
														highByte[1],lowByte[1],
														highByte[2],lowByte[2],
														highByte[3],lowByte[3]};

   i2c.send(buffer,8);

}


//-------------writeEeprom---------------------
/** @~english 
* @brief Writes a value to a specific channel and its EEPROM.
*
* With this function you can write a analog signal to a specific channel and its EEPROM.
* @param channel Channel for analog value
* @param value value for selected channel
*
* @~german 
* @brief Bestimmt den Wert eines ausgewählten Channels und schreibt den Wert in das EEPROM
*
* Mit dieser Funktion kann man ein analoges Signal in einen bestimmten Kanal (Register und dessen EEPROM) schreiben.
* @param channel Kanal für den analogen Wert
* @param value Wert für den ausgewählten Kanal
*
*/

void gnublin_module_dac::writeEeprom(int channel, int value) {
	_channel[channel] = value;   
	char lowByte=0x00;
	char highByte=0x00;

	lowByte = (char) (value & 0xff);
	highByte = (char) ((value >> 8) & 0xff);
	if (_vRef[channel]) highByte = highByte | 0x80;
	if (_gain[channel]) highByte = highByte | 0x10;

	unsigned char buffer[3];
	buffer[1] = highByte; 
	buffer[2] = lowByte;
	
	switch (channel) {
		case 0: buffer[0]=0x58;	break;
		case 1: buffer[0]=0x5a;	break;
		case 2: buffer[0]=0x5c;	break;
		case 3: buffer[0]=0x5e;	break;
	}

  i2c.send(buffer,3);
	//i2c.close_fd();
	usleep(50000);
}

//-------------write---------------------
/** @~english 
* @brief Writes a value to a specific channel. No EEPROM access.
*
* With this function you can write a analog signal to a specific channel.
* @param channel Channel for analog value
* @param value value for selected channel
*
* @~german 
* @brief Bestimmt den Wert eines ausgewählten Channels. Kein EEPROM.
*
* Mit dieser Funktion kann man ein analoges Signal in einen bestimmten Kanal schreiben. Wert wird nur in das Register geschrieben, nicht in das EEPROM.
* @param channel Kanal für den analogen Wert
* @param value Wert für den ausgewählten Kanal
*
*/

void gnublin_module_dac::write(int channel, int value) {
	_channel[channel] = value;   
	char lowByte=0x00;
	char highByte=0x00;

	lowByte = (char) (value & 0xff);
	highByte = (char) ((value >> 8) & 0xff);
	if (_vRef[channel]) highByte = highByte | 0x80;
	if (_gain[channel]) highByte = highByte | 0x10;
	
	unsigned char buffer[3];
	buffer[1] = highByte; 
	buffer[2] = lowByte;
	
	switch (channel) {
		case 0: buffer[0]=0x50;	break;
		case 1: buffer[0]=0x52;	break;
		case 2: buffer[0]=0x54;	break;
		case 3: buffer[0]=0x56;	break;
	}
	
  i2c.send(buffer,3);
	usleep(50000);
}



//-------------read---------------------
/** @~english 
* @brief Reads the value from the register of a specific channel
*
* With this function you can read the digital value from the register of a specific channel. Maximum value is 4095. 
* @param channel Select channel for reading
*
* @return Returns value of the register
*
* @~german 
* @brief Liest den Wert des Registers eines ausgewählten Kanals
*
* Mit dieser Funktion kann man den Zustand des Register eines bestimmten Kanals auslesen. Maximaler Wert ist 4095.
* @param channel Kanal, von welchem gelesen werden soll.
*
* @return Gibt den Wert des Registers zurück
*
*/

int gnublin_module_dac::read(int channel) {
	int value=0;
	unsigned char rx_buf[24]={0};
	error_flag=false;
	
	i2c.receive(rx_buf, 24);
	
	value = value | rx_buf[1+6*channel];
	value = value & 0x0f;
	value = value << 8;
	value = value | rx_buf[2+6*channel];
		
	return value;
}

//-------------gain---------------------
/** @~english 
* @brief Set gain. 1x or 2x.
*
* With this function you can set the gain. No EEPROM.
* @param channel Channel for gain.
* @param value 0 = 1x, 1 = 2x.
*
* @~german 
* @brief Bestimmt die Verstärkung. 1x oder 2x.
*
* Mit dieser Funktion kann man die Verstärkung festlegen. Es wird ausschließlich ins Register geschrieben. Kein EEPROM. 
* @param channel Kanal für die Verstärkung
* @param value 0 = 1x Verstärkung, 1 = 2x Verstärkung
*
*/

void gnublin_module_dac::gain(int channel, int val) {
	_gain[channel] = val;   
	write(channel, _channel[channel]);
}

//-------------vRef---------------------
/** @~english 
* @brief Sets vRef. External or internal. No EEPROM, fast access.
*
* Sets vRef on external or internal. external is 3.3V. Internal is 2.048V. Since no EEPROM access takes place, to ensure quick access.
* @param channel Channel for vRef
* @param value 0 = External Ref, 1 = Internal Ref
*
* @~german 
* @brief Bestimmt vRef. External oder Internal. Kein EEPROM, schneller Zugriff.
*
* Mit dieser Funktion kann die Spannungsreferenz festgelegt werden. External wäre 3,3V und Internal 2,048. Da kein EEPROM Zugriff stattfindet, ist ein schneller Zugriff gewährleistet.
* @param channel Kanal auf welchen die Spannungsreferenz angewandt werden soll
* @param value 0 = External Ref, 1 = Internal Ref
*
*/

void gnublin_module_dac::vRef(int channel, int val) {
	_vRef[channel] = val; 
	write(channel, _channel[channel]);
}

//-------------gainEeprom---------------------
/** @~english 
* @brief Set gain. 1x or 2x. Writes into EEPROM.
*
* With this function you can set the gain. Writes gain value also to EEPROM.
* @param channel Channel for gain.
* @param value 0 = 1x, 1 = 2x.
*
* @~german 
* @brief Bestimmt die Verstärkung und schreibt ins Register und EEPROM.
*
* Mit dieser Funktion kann man die Verstärkung festlegen. Es wird ins Register und ins EEPROM geschrieben. 
* @param channel Kanal für Verstärkung
* @param value 0 = 1x, 1 = 2x.
*
*/

void gnublin_module_dac::gainEeprom(int channel, int val) {
	_gain[channel] = val;   
	writeEeprom(channel, _channel[channel]);
}


//-------------vRef---------------------
/** @~english 
* @brief Sets vRef. External or internal. Writes to EEPROM. 
*
* Sets vRef on external or internal. external is 3.3V. Internal is 2.048V. Also writes to EEPROM.
* @param channel Channel for vRef
* @param value 0 = External Ref, 1 = Internal Ref
*
* @~german 
* @brief Bestimmt vRef. External oder Internal. Schreibt ins EEPROM.
*
* Mit dieser Funktion kann die Spannungsreferenz festgelegt werden. External wäre 3,3V und Internal 2,048. Schreibt ins Register und ins EEPROM.
* @param channel Kanal auf welchen die Spannungsreferenz angewandt werden soll
* @param value 0 = External Ref, 1 = Internal Ref
*
*/


void gnublin_module_dac::vRefEeprom(int channel, int val) {
	_vRef[channel] = val; 
	writeEeprom(channel, _channel[channel]);
}



