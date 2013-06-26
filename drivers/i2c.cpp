#include "i2c.h"

//*******************************************************************
//Class for accessing GNUBLIN i2c Bus
//*******************************************************************

//------------------local defines-----------------
/** @~english 
* @brief creates macro reference for default device "/dev/i2c-1"
*
* @~german 
* @brief definiert das Standard i2c device "/dev/i2c-1"
*
*/
#define DEFAULTDEVICE "/dev/i2c-1"

//------------------Konstruktor------------------
/** @~english
* @brief initalizes the i2c bus. Sets the devicefile to "/dev/i2c-1"
*
* @~german
* @brief initialisiert den i2c Bus. Setzt das standard i2c device auf "/dev/i2c-1"
*
*/
gnublin_i2c::gnublin_i2c() 
{
	init(DEFAULTDEVICE, -1);
}

//------------------Konstruktor------------------
/** @~english
* @brief initalizes the i2c bus. Sets the devicefile to "/dev/i2c-1"
* @param Address new I2C slave adress
*
* @~german
* @brief initialisiert den i2c Bus. Setzt das standard i2c device auf "/dev/i2c-1"
* @param Address neue I2C slave Adresse
*
*/
gnublin_i2c::gnublin_i2c(int Address) 
{
	init(DEFAULTDEVICE, Address);
}

//------------------Konstruktor------------------
/** @~english
* @brief initalizes the i2c bus. 
* @param Devicefile new I2C device file, e.g. "/dev/i2c-2"
* @param Address new I2C slave adress
*
* @~german
* @brief initialisiert den i2c Bus.
* @param Devicefile neues I2C device file, z.B. "/dev/i2c-2"
* @param Address neue I2C slave Adresse
*
*/
gnublin_i2c::gnublin_i2c(std::string Devicefile, int Address)
{
	init(Devicefile, Address);
} 

//------------------destructor------------------
/** @~english
* @brief Closes the file handle 
*
* @~german
* @brief Schließt die Datei 
*
*/
gnublin_i2c::~gnublin_i2c()
{
	close_fd();
}

//------------------init------------------
/** @~english
* @brief Called by the constructors to initialize class variables.
* @param Devicefile new I2C device file, e.g. "/dev/i2c-2"
* @param Address new I2C slave adress
*
* @~german
* @brief Wird von den Konstruktoren der Klasse aufgerufen um die Variablen zu initialisieren.
* @param Devicefile neues I2C device file, z.B. "/dev/i2c-2"
* @param Address neue I2C slave Adresse
*
*/
void gnublin_i2c::init(std::string Devicefile, int Address) 
{
	devicefile=Devicefile;
        slave_address = Address;
	error_flag=false;
        fd = 0;
}

//------------------error messaging------------------
/** @~english
* @brief Called by the send and receive Methods when an Error occures
*
* @param message String contents that describe the error.
* @return -1
*
* @~german
* @brief Wird von den send und receive Methoden aufgerufen, wenn ein Fehler auftritt
*
* @param message String der den Fehler beschreibt.
* @return -1
*
*/
int gnublin_i2c::errorMsg(std::string message)
{
	ErrorMessage=message;
	error_flag=true; 
	close_fd();
	return -1; 
}

//------------------close file descriptor------------------
/** @~english
* @brief Closes the file if open and resets the variable.
*
* @~german
* @brief Schließt die Datei wenn sie offen ist und setzt die Variable zurück.
*
*/
void gnublin_i2c::close_fd()
{
	if (fd) {
		close(fd);
		fd = 0;
	}
}

//------------------open file descriptor------------------
/** @~english
* @brief Opens the devicefile. If a file is already open it is closed first.  A new file is opened
* and io operations defined based on the class values for devicefile 
* and slave_address.
*
* @return success: 0, failure: -1
*
* @~german
* @brief Öffnet die Geräte Datei. Wenn eine Datei bereits geöffnet ist, wird sie zunächst geschlossen. 
* Eine neue Datei wird geöffnet und IO-Operationen werden auf Basis der Klassenvariablen
* devicefile und slave_address definiert.
*
* @return Erfolg: 0, Misserfolg: -1
*
*/
int gnublin_i2c::open_fd() 
{
	error_flag = false;

	if (fd) { 
		close_fd(); 
		fd = 0;
	}

	if (slave_address == -1) 
		return errorMsg("ERROR slave address is not set\n");

	if ((fd = open(devicefile.c_str(), O_RDWR)) < 0) 
		return errorMsg("ERROR opening: " + devicefile + "\n");

	if (ioctl(fd, I2C_SLAVE, slave_address) < 0) 
		return errorMsg("ERROR address: " + numberToString(slave_address) + "\n");

	return 0;
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
* @return failure: -1 
*
* @~german 
* @brief Setzt die i2c slave Adresse
*
* Mit dieser Funktion kann die individuelle I2C Slave-Adresse gesetzt werden.
* @param Address neue I2C slave Adresse
* @return failure: -1 
*/
int gnublin_i2c::setAddress(int Address){
	slave_address = Address;
	return open_fd();
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
* @return failure: -1 
*
* @~german
* @brief setzt die I2C Device Datei. Standard ist die "/dev/i2c-1"
*
* Diese Funktion setzt die Geräte Datei, auf die man zugreifen möchte. Standardmäßig ist bereits "/dev/i2c-1" gesetzt.
* @param filename Dateipfad zur Geräte Datei, z.B. "/dev/i2c-0"
* @return failure: -1 
*/
int gnublin_i2c::setDevicefile(std::string filename){
	devicefile = filename;
	return open_fd();
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

	if (RxBuf == 0)
		return errorMsg("Receive method received a null TxBuf pointer.\n");
	if (length < 1)
		return errorMsg("Receive method received an invalid buffer length.\n");

	if (!fd)
		 if (open_fd() == -1)
			  return -1;

	error_flag=false;	

	if (read(fd, RxBuf, length) != length)
		return errorMsg("i2c read error! Address: " + numberToString(slave_address) + " dev file: " + devicefile + "\n");		

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

	if (RxBuf == 0)
		return errorMsg("Receive method received a null TxBuf pointer.\n");
	if (length < 1)
		return errorMsg("Receive method received an invalid buffer length.\n");

	if (!fd)
		if (open_fd() == -1)
			  return -1;

	error_flag=false;

	if (write(fd, &RegisterAddress, 1) != 1)
  		return errorMsg("i2c write error!\n");

	if (read(fd, RxBuf, length) != length)
		return errorMsg("i2c read error! Address: " + numberToString(slave_address) + " dev file: " + devicefile + "\n");

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

	if (TxBuf == 0)
		return errorMsg("Send method received a null TxBuf pointer.\n");
	if (length < 1)
		return errorMsg("Send method received an invalid buffer length.\n");

	if (!fd)
		if (open_fd() == -1)
			  return -1;

	error_flag=false;	

	if(write(fd, TxBuf, length) != length)
		return errorMsg("i2c write error!\n");

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
	int i;
	unsigned char data[length+1];
	data[0]=RegisterAddress;
	
	for ( i = 0; i < length ; i++ ) {
		data[i+1] = TxBuf[i];
	}

	if (TxBuf == 0)
		return errorMsg("Send method received a null TxBuf pointer.\n");
	if (length < 1)
		return errorMsg("Send method received an invalid buffer length.\n");

	if (!fd)
		if (open_fd() == -1)
			  return -1;

	error_flag=false;	

/*	if (send(RegisterAddress) == -1)
		return -1;
*/
	if(write(fd, data, length+1) != length+1)
		return errorMsg("i2c write error!\n");

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
int gnublin_i2c::send(unsigned char value){

	if (!fd)
		if (open_fd() == -1)
			  return -1;

	error_flag=false;

	if(write(fd, &value, 1) != 1)
		return errorMsg("i2c write error!\n");

	return 1;
}
