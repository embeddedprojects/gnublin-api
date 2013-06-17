#include "i2c.h"

//*******************************************************************
//Class for accessing GNUBLIN i2c Bus
//*******************************************************************

//------------------local defines-----------------
/** @~english 
* @brief creates macro reference for default device "/dev/i2c-1"
*
* @~german 
* @brief schafft Makro Referenz für Standard-Gerät "/ dev/i2c-1"
*
*/
#define DEFAULTDEVICE "/dev/i2c-1"

//------------------Konstruktor------------------
/** @~english
* @brief Sets the devicefile to "/dev/i2c-1"
*
* @~german
* @brief Setzt das devicefile auf standardmäßig "/dev/i2c-1"
*
*/
gnublin_i2c::gnublin_i2c() : gnublin_driver()
{
	init(DEFAULTDEVICE, -1);
}

//------------------Konstruktor------------------
/** @~english
* @brief Sets the devicefile to "/dev/i2c-1"
*
* @~german
* @brief Setzt das devicefile auf standardmäßig "/dev/i2c-1"
*
*/
gnublin_i2c::gnublin_i2c(int Address) : gnublin_driver()
{
	init(DEFAULTDEVICE, Address);
}

//------------------Konstruktor------------------
/** @~english
* @brief Sets the devicefile to "/dev/i2c-1"
*
* @~german
* @brief Setzt das devicefile auf standardmäßig "/dev/i2c-1"
*
*/
gnublin_i2c::gnublin_i2c(std::string Devicefile, int Address) : gnublin_driver()
{
	init(Devicefile, Address);
} 

//------------------destructor------------------
/** @~english
* @brief Closes the file handle 
*
* @~german
* @brief Schließt die Datei Griff
*
*/
gnublin_i2c::~gnublin_i2c()
{
	close_fd();
}

//------------------init------------------
/** @~english
* @brief Called by the constructors to initialize class variables.
*
* @~german
* @brief Wird von den Konstruktoren der Klasse Variablen zu initialisieren.
*
*/
void gnublin_i2c::init(std::string Devicefile, int Address) 
{
	devicefile=Devicefile;
        slave_address = Address;
        fd = 0;
}

//------------------Error message------------------
/** @~english
* @brief Closes the file if open and resets the variable.
* called from base class when error message is set
*
* @~german
* @brief Schließt die Datei, wenn offen und setzt die Variable.
* Wird aufgerufen, wenn Fehlermeldung Basisklasse eingestellt ist.
*
*/
void gnublin_i2c::onError()
{
   close_fd();
}

//------------------close file descriptor------------------
/** @~english
* @brief Closes the file if open and resets the variable.
*
* @~german
* @brief Schließt die Datei, wenn offen und setzt die Variable.
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
* @brief if a file is already open it is closed first.  A new file is opened
* and io operations defined based on the class values for devicefile 
* and slave_address.
*
* @return failure: -1
*
* @~german
* @brief wenn eine Datei bereits geöffnet ist, wird es zunächst geschlossen. 
* Eine neue Datei wird geöffnet und IO-Operationen werden auf Basis der Klasse
* Werte für Device-Datei und Slave_Address definiert.
*
* @return failure: -1
*
*/
int gnublin_i2c::open_fd() 
{
    clearError();

    if (fd) { 
        close_fd(); 
        fd = 0;
    }

    if (slave_address == -1) 
        return setErrorMessage("ERROR slave address is not set\n");

    if ((fd = open(devicefile.c_str(), O_RDWR)) < 0) 
        return setErrorMessage("ERROR opening: " + devicefile + "\n");

    if (ioctl(fd, I2C_SLAVE, slave_address) < 0) 
        return setErrorMessage("ERROR address: " + numberToString(slave_address) + "\n");

    return 0;
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
		return setErrorMessage("Receive method received a null TxBuf pointer.\n");
	if (length < 1)
		return setErrorMessage("Receive method received an invalid buffer length.\n");

	if (!fd)
		if (open_fd() == -1)
			return -1;

	clearError();	

	if (read(fd, RxBuf, length) != length)
		return setErrorMessage("i2c read error! Address: " + numberToString(slave_address) + " dev file: " + devicefile + "\n");		

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
		return setErrorMessage("Receive method received a null TxBuf pointer.\n");
	if (length < 1)
		return setErrorMessage("Receive method received an invalid buffer length.\n");

	if (!fd)
		if (open_fd() == -1)
			return -1;

	clearError();

	if (write(fd, &RegisterAddress, 1) != 1)
  		return setErrorMessage("i2c write error!\n");

	if (read(fd, RxBuf, length) != length)
		return setErrorMessage("i2c read error! Address: " + numberToString(slave_address) + " dev file: " + devicefile + "\n");

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
		return setErrorMessage("Send method received a null TxBuf pointer.\n");
	if (length < 1)
		return setErrorMessage("Send method received an invalid buffer length.\n");

	if (!fd)
		if (open_fd() == -1)
			  return -1;

        clearError();

	if(write(fd, TxBuf, length) != length)
		return setErrorMessage("i2c write error!\n");

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

	if (TxBuf == 0)
		return setErrorMessage("Send method received a null TxBuf pointer.\n");
	if (length < 1)
		return setErrorMessage("Send method received an invalid buffer length.\n");

	if (!fd)
		if (open_fd() == -1)
			return -1;

	clearError();

	if (send(RegisterAddress) == -1)
		return -1;

	if(write(fd, TxBuf, length) != length)
		return setErrorMessage("i2c write error!\n");

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

	clearError();

	if(write(fd, &value, 1) != 1)
		return setErrorMessage("i2c write error!\n");

	return 1;
}
