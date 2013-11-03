//********************************************
//GNUBLIN API -- MAIN FILE
//build date: 11/03/13 18:42
//******************************************** 

#include "gnublin.h"


//Converting string to number
int stringToNumber(std::string str){
	std::stringstream  strin;
	int var;

	strin << str;
	strin >> var;
	
	return var;
}

//Converting a number to string
std::string numberToString(int num){
	std::ostringstream strout;
	std::string str;

	strout << num;
	str = strout.str();

	return str;
}

//Converting string wich repesenting a hexnumber to number
int hexstringToNumber(std::string str){
	std::stringstream  strin;
	int var;

	strin << std::hex << str;
	strin >> var;
	
	return var;
}

/** @~english 
* @brief Reset the ErrorFlag.
*
* @~german 
* @brief Setzt das ErrorFlag zurück.
*
*/
gnublin_gpio::gnublin_gpio(){
	error_flag = false;
}



/** @~english 
* @brief Removes the GPIO. 
*
* Removes the GPIO from the filesystem, after that, you cannot access the pin.
* @return bool error_flag
*
* @~german 
* @brief Entferne GPIO
*
* Entfernt den GPIO aus dem Filesystem, es kann kein Zugriff mehr auf diesen erfolgen.
* @return bool error_flag
*/
int gnublin_gpio::unexport(int pin){
	std::string pin_str = numberToString(pin);
	std::string dir = "/sys/class/gpio/unexport";
	std::ofstream file (dir.c_str());
	if (file < 0) {
		error_flag = true;
		return -1;
	}
	file << pin_str;
	file.close();
	error_flag = false;
	return 1;
}


/** @~english 
* @brief Returns the error flag. 
*
* If something went wrong, the flag is true.
* @return bool error_flag
*
* @~german 
* @brief Gibt das Error Flag zurück.
*
* Falls das Error Flag in der Klasse gesetzt wurde, wird true zurück gegeben, anderenfalls false.
* @return bool error_flag
*/
bool gnublin_gpio::fail(){
	return error_flag;
}

/** @~english 
* @brief Get the last Error Message.
*
* This Funktion returns the last Error Message, which occurred in that Class.
* @return ErrorMessage as c-string
*
* @~german 
* @brief Gibt die letzte Error Nachricht zurück.
*
* Diese Funktion gibt die Letzte Error Nachricht zurück, welche in dieser Klasse gespeichert wurde.
* @return ErrorMessage als c-string
*/
const char *gnublin_gpio::getErrorMessage(){
	return ErrorMessage.c_str();
}

/** @~english 
* @brief Change the PinMode.
*
* With this function you can set the pin as input or output. <br>
* This must be done before all other operations. <br><br>
* Hint: GPIO 4 can only be used as output.
* @param pin Pin number to change the mode
* @param direction direction of the pin (OUTPUT, INPUT)
* @return success: 1, failure: -1
*
* @~german 
* @brief Pin Modi festlegen.
*
* Mit dieser Funktion kann der Modi des Pins festgelegt werden, also ob er als Eingang oder Ausgang genutzt werden soll. <br>
* Vor allen anderen Zugriffen muss diese Funktion ausgeführt werden. <br> <br>
* Hinweis: GPIO 4 kann auf dem GNUBLIN board nur als Ausgang genutzt werden. 
* @param pin Nummer des GPIO-Pins
* @param direction Richtung des Pins (OUTPUT, INPUT) 
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_gpio::pinMode(int pin, std::string direction){
	#if (BOARD != RASPBERRY_PI && BEAGLEBONE_BLACK)
	if (pin == 4 && direction == "out"){
		error_flag = true;
		return -1;
	}
	#endif
	std::string pin_str = numberToString(pin);
	std::string dir = "/sys/class/gpio/export";
	std::ofstream file (dir.c_str());
	if (file < 0) {
		error_flag = true;
		return -1;
	}
	file << pin;
	file.close();

	dir = "/sys/class/gpio/gpio" + pin_str + "/direction";

	file.open(dir.c_str());
	if (file < 0) {
		error_flag = true;
		return -1;
	}
	file << direction;
	file.close();
	error_flag = false;
	return 1;
}


/** @~english 
* @brief Write Pin.
*
* Set the Pin to the given value. 
* @param pin Pin number to change the mode
* @param value Value of the Pin (LOW, HIGH)
* @return success: 1, failure: -1
*
* @~german 
* @brief Pin schreiben.
*
* Den GPIO-Pin auf einen bestimmten Wert setzen. 
* @param pin Nummer des entsprechenden GPIO-Pins
* @param value Wert auf den der Pin gesetzt werden soll (LOW, HIGH) 
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_gpio::digitalWrite(int pin, int value){
	#if (BOARD != RASPBERRY_PI && BEAGLEBONE_BLACK)
	if (pin == 4){
		error_flag = true;
		return -1;
	}
	#endif
	if (value != 0 && value != 1){
		error_flag = true;
		return -1;
	}
	std::string value_str = numberToString(value);
	std::string pin_str = numberToString(pin);
	std::string dir = "/sys/class/gpio/gpio" + pin_str + "/value";

	std::ofstream file (dir.c_str());
	if (file < 0) {
		error_flag = true;
		return -1;
	}
	file << value_str;
	file.close();
	error_flag = false;
	return 1;
}

/** @~english 
* @brief Read Pin.
*
* Reads the value of the given pin. 
* @param pin Pin number to read
* @return Value of the GPIO-Pin (0,1), -1 in case of failure 
*
* @~german 
* @brief Pin lesen.
*
* Abfragen des GPIO-Wertes. 
* @param pin Nummer des entsprechenden GPIO-Pins
* @return Wert des GPIO-Pins (0,1), -1 im Fehlerfall 
*/
int gnublin_gpio::digitalRead(int pin) {
	std::string value;
	
	std::string pin_str = numberToString(pin);
	std::string device = "/sys/class/gpio/gpio" + pin_str + "/value";
	std::ifstream file(device.c_str());
	if (file < 0){
		error_flag = true;
		return -1;
	}
	file >> value;
	file.close();
	error_flag = false;
	return stringToNumber(value);
	
}


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


//***************************************************************************
// Class for accessing the SPI-Bus
//***************************************************************************

/**
* @~english
* @brief Loads the standard SPI driver if necessary.
*
* Default chipselect:
* GNUBLIN: CS = 11
* RASPBERRY PI: CS = 0
* @~german
* @brief Die Standart SPI-Treiber werden geladen, wenn sie noch nicht vorhanden sind.
*
* Default chipselect:
* GNUBLIN: CS = 11
* RASPBERRY PI: CS = 0
* BEAGLEBONE BLACK: CS = 0
*/
gnublin_spi::gnublin_spi(){
	error_flag = false;
	#if (BOARD == RASPBERRY_PI)
	std::string device = "/dev/spidev0.0";
	#elif (BOARD == BEAGLEBONE_BLACK)
	std::string device = "/dev/spidev1.0";
	#else
	std::string device = "/dev/spidev0.11";
	#endif
	fd = open(device.c_str(), O_RDWR);
	if (fd < 0) {
		#if BOARD == RASPBERRY_PI
		system("modprobe spi-bcm2708");
		#else
		system("modprobe spidev cs_pin=11");
		#endif
		sleep(1);
		fd = open(device.c_str(), O_RDWR);
	}
}


//******************** destructor *******************************************

gnublin_spi::~gnublin_spi(){
	close(fd);
}


//******************** fail() ***********************************************
/**
* @~english
* @brief Returns the errorflag to detect error in the previous called method.
*
* @return error_flag
*
* @~german
* @brief Gibt das errorflag zurück, um Fehler in der zuvor aufgerugfenen Methode zu erkennen.
*
* @return error_flag
*/
bool gnublin_spi::fail(){
	return error_flag;
}


//-------------get Error Message-------------
/**
* @~english
* @brief Returns the ErrorMessage of the previous error if one exist.
*
* @return ErrorMessage as C-String
*
* @~german
* @brief Gibt die Fehlernachricht des zuvor aufgetretenen Fehlers zurück, wenn weine exisitert.
*
* @return ErrorMessage als C-String
*/
const char *gnublin_spi::getErrorMessage(){
	return ErrorMessage.c_str();
}


//*********************** setCS *********************************************

/**
* @~english
* @brief Set the custom chipselect pin
*
* @param cs Number of the chipselect-pin
* @return 1 by success, -1 by failure
*
* @~german
* @brief Setzt den benutzerdefinierten Chipselect-Pin.
*
* @param cs Nummer des Chipselect-Pin
* @return 1 bei Erfolg, -1 im Fehlerfall
*/
int gnublin_spi::setCS(int cs){
	std::string cs_str = numberToString(cs);
	std::string device = "/dev/spidev0." + cs_str;
	fd = open(device.c_str(), O_RDWR);
	if (fd < 0) {
		#if (BOARD == RASPBERRY_PI)
		std::string command = "modprobe spi-bcm2708 cs_pin=" + cs_str;
		#else
		std::string command = "modprobe spidev cs_pin=" + cs_str;
		#endif
		system(command.c_str());
		sleep(1);
		fd = open(device.c_str(), O_RDWR);
		if (fd < 0){
			error_flag = true;
			return -1;
		}
	}
	error_flag = false;
	return 1;
}


//******************** set Mode *********************************************

/**
* @~english
* @brief Set the SPI-mode
*
* @param mode Number of the SPI-Mode
* @return 1 by success, -1 by failure
*
* @~german
* @brief Setzt den SPI-Modus
*
* @param mode Nummer SPI-Modus
* @return 1 bei Erfolg, -1 im Fehlerfall
*/
int gnublin_spi::setMode(unsigned char mode){
	if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0){
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return 1;
}


//***************** getMode *************************************************

/**
* @~english
* @brief Returns the set SPI-Mode
*
* @return Number of the SPI-mode
*
* @~german
* @brief Gibt den eingestellten SPI-Modus zurück
*
* @return Nummer des SPI-Modus
*/
int gnublin_spi::getMode(){
	__u8 mode;
	if (ioctl(fd, SPI_IOC_RD_MODE, &mode) < 0){
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return mode;
}


//******************** setLSB ***********************************************

/**
* @~english
* @brief Set the LSB-mode
*
* @param lsb 0: MSB first, 1 LSB first
* @return 1 by success, -1 by failure
*
* @~german
* @brief Setzt den LSB-Modus.
*
* @param lsb 0: MSB zuerst; 1 LSB zuerst
* @return 1 bei Erfolg, -1 im Fehlerfall
*/
int gnublin_spi::setLSB(unsigned char lsb){
	if (ioctl(fd, SPI_IOC_WR_LSB_FIRST, &lsb) < 0){
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return 1;
}


//************************ getLSB() *****************************************

/**
* @~english
* @brief Returns the set LSB-Mode
*
* @return 0: MSB first, 1 LSB first
*
* @~german
* @brief Gibt den eingestellten LSB-Modus zurück.
*
* @return 0: MSB zuerst; 1 LSB zuerst
*/
int gnublin_spi::getLSB(){
	__u8 lsb;
	if (ioctl(fd, SPI_IOC_RD_LSB_FIRST, &lsb) < 0) {
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return lsb;
}


//*********************** setLength() ***************************************

/**
* @~english
* @brief Set the lenght of the words which will be send
*
* @param bits Number of bits of each word
* @return 1 by success, -1 by failure
*
* @~german
* @brief Legt die Länge der gesendeten Wörter fest
*
* @param bits Anzahl der Bits je Word
* @return 1 bei Erfolg, -1 im Fehlerfall
*/
int gnublin_spi::setLength(unsigned char bits){
	if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0){
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return 1;
}


//************************ getLength() **************************************

/**
* @~english
* @brief Returns the length of the words in bits
*
* @return Number of bits of each word
*
* @~german
* @brief Gibt die Anzahl von Bits je Wort zurück.
*
* @return anzahl der Bits je Wort
*/
int gnublin_spi::getLength(){
	__u8 bits;
	if (ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits) < 0){
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return bits;
}


//************************* setSpeed ****************************************

/**
* @~english
* @brief Set the speed of the SPI-Bus
*
* @param speed Speed in Hz
* @return 1 by success, -1 by failure
*
* @~german
* @brief Legt die Geschwindigkeit des SPI-Buses fest.
*
* @param speed Geschwindigkeit in Hz
* @return 1 bei Erfolg, -1 im Fehlerfall
*/
int gnublin_spi::setSpeed(unsigned int speed){
	if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0){
		error_flag = true;
		return -1;
	}
	error_flag = true;
	return 1;
}


//************************* getSpeed() **************************************

/**
* @~english
* @brief Returns the speed of the SPI-Bus
*
* @return SPeed in Hz
*
* @~german
* @brief Gibt die Geschwindigkeit des SPI-Buses zurück.
*
* @return Geschwindigkeit in Hz
*/
int gnublin_spi::getSpeed(){
	__u32 speed;
	if (ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed) < 0){
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return speed;
}

//**************************** receive **************************************

/**
* @~english
* @brief Receive data from SPI Bus
*
* @param buffer Buffer for recived datas
* @param len Length of recived data
* @return 1 by success, -1 by failure
*
* @~german
* @brief Empfängt Daten über den SPI-Bus
*
* @param buffer Buffer für die empfangenen Daten
* @param len Anzahl der zu empfangenden Zeichen
* @return 1 bei Erfolg, -1 im Fehlerfall
*/
int gnublin_spi::receive(char* buffer, int len){
	if (read(fd, buffer, len) < 0) {
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return 1;
}

//*************************** send() ****************************************

/**
* @~english
* @brief Send data over the SPI Bus
*
* @param tx Datas which will be send
* @param length Length of datas
* @return 1 by success, -1 by failure
*
* @~german
* @brief Sendet Daten über den SPI-Bus
*
* @param tx Zu sendende Daten
* @param length Anzahl der zu sendenden Zeichen
* @return 1 bei Erfolg, -1 im Fehlerfall
*/
int gnublin_spi::send(unsigned char* tx, int length){
	int status;
	struct spi_ioc_transfer	xfer;
	xfer.tx_buf = (unsigned long)tx;
	xfer.len = length;
	xfer.rx_buf = 0;
	xfer.delay_usecs = 0;
	xfer.speed_hz = 0;
	xfer.bits_per_word = 0;
	status = ioctl(fd, SPI_IOC_MESSAGE(1), &xfer);
	if ( status < 0){
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return 1;
}


//****************************** message() ********************************

/**
* @~english
* @brief Send and recive data over the SPI-Bus (half duplex)
*
* @param tx Data which will be send
* @param tx_length Length of data which will be send
* @param rx Buffer for recived datas
* @param rx_length length of recived datas
* @return 1 by success, -1 by failure
*
* @~german
* @brief Sendet und empfängt daten über den SPI-Bus (halb duplex).
*
* @param tx Zu sendende Daten
* @param tx_length Anzahl der zu sendenden Zeichen
* @param rx Buffer für den Datenempfang
* @param rx_length Anzahl der zu empfangenden Zeichen
* @return 1 bei Erfolg, -1 im Fehlerfall
*/
int gnublin_spi::message(unsigned char* tx, int tx_length, unsigned char* rx, int rx_length){
	int status;
	struct spi_ioc_transfer xfer[2];
	xfer[0].tx_buf = (unsigned long) tx;
	xfer[0].len = tx_length;
	xfer[0].rx_buf = 0;
	xfer[0].delay_usecs = 0;
	xfer[0].speed_hz = 0;
	xfer[0].bits_per_word = 0;
	
	xfer[1].rx_buf = (unsigned long) rx;
	xfer[1].len = rx_length;
	xfer[1].tx_buf = 0;
	xfer[1].delay_usecs = 0;
	xfer[1].speed_hz = 0;
	xfer[1].bits_per_word = 0;

	status = ioctl(fd, SPI_IOC_MESSAGE(2), xfer);
	if (status < 0){
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return 1;
}


#if (BOARD != RASPBERRY_PI)
//****************************************************************************
// Class for easy acces to the GPAs
//****************************************************************************
/** @~english 
* @brief Opens the file stream and loads the kernel object lpc313x_adc.
*
* @~german 
* @brief Erzeugt den Datei stream und lädt das Kernelmodul lpc313x_adc.
*
*/
gnublin_adc::gnublin_adc(){
	#if (BOARD == BEAGLEBONE_BLACK)
	std::ifstream file("/sys/devices/ocp.2/helper.14/AIN1");
	if (file.fail()) {
		std::ofstream file2("/sys/devices/bone_capemgr.14/slots");
		file2 << "cape-bone-iio";
		file2.close();
		sleep(1);
	}
	#else
	std::ifstream file("/dev/lpc313x_adc");
	if (file.fail()) {
		system("modprobe lpc313x_adc");
		sleep(1);
	}
	#endif
	file.close();
	error_flag = false;
}

//-------------fail-------------
/** @~english 
* @brief Returns the error flag. 
*
* If something went wrong, the flag is true.
* @return bool error_flag
*
* @~german 
* @brief Gibt das Error Flag zurück.
*
* Falls das Error Flag in der Klasse gesetzt wurde, wird true zurück gegeben, anderenfalls false.
* @return bool error_flag
*/
bool gnublin_adc::fail(){
	return error_flag;
}


//-------------getErrorMessage-------------
/** @~english 
* @brief Get the last Error Message.
*
* This Funktion returns the last Error Message, which occurred in that Class.
* @return ErrorMessage as c-string
*
* @~german 
* @brief Gibt die letzte Error Nachricht zurück.
*
* Diese Funktion gibt die Letzte Error Nachricht zurück, welche in dieser Klasse gespeichert wurde.
* @return ErrorMessage als c-string
*/
const char *gnublin_adc::getErrorMessage(){
	return ErrorMessage.c_str();
}

#if (BOARD != BEAGLEBONE_BLACK)
//-------------getValue-------------
/** @~english 
* @brief Get Value.
*
* This Funktion returns the Value from the register of the given pin.
* @param pin The pin, which should be used
* @return Value of the Pin - in failure: -1
*
* @~german 
* @brief Gibt den Wert des Pins zurück.
*
* Mit dieser Funktion erhält man den gemessenen Rohwert des angegebenen Pins, also den Wert des Registers. 
* @param pin Gibt den ADC-Pin an, von dem gemessen werden soll 
* @return Wert des ADCs, im Fehlerfall -1
*/
int gnublin_adc::getValue(int pin){
	std::string value;
	
	std::string pin_str = numberToString(pin);
	std::string device = "/dev/lpc313x_adc";
	std::ofstream file(device.c_str());
	if (file < 0) {
		error_flag = true;
		return -1;
	}
	file << pin_str;
	file.close();
	std::ifstream dev_file(device.c_str());
	dev_file >> value;
	dev_file.close();
	error_flag = false;
	return hexstringToNumber(value);
}
#endif


//-------------getVoltage-------------
/** @~english 
* @brief Get Voltage.
*
* This Funktion returns the Voltage of the given pin.
* @param pin The pin, which should be used
* @return Voltage of the Pin - in failure: -1
*
* @~german 
* @brief Ließt Spannung.
*
* Liefert den gemessenen Wert in mV. 
* @param pin Gibt den ADC-Pin an, von dem gemessen werden soll 
* @return Spannung des ADCs in mV, im Fehlerfall -1
*/
int gnublin_adc::getVoltage(int pin){
	#if (BOARD == BEAGLEBONE_BLACK)
	int value;
	std::stringstream ss;
	ss << pin;
	std::string device = "/sys/devices/ocp.2/helper.14/AIN" + ss.str();
	for (int i=0; i<2; i++){
		std::ifstream dev_file(device.c_str());
		dev_file >> value;
		dev_file.close();
	}
	#else
	int value = getValue(pin);
	value = value*825/256;
	#endif
	return value;
}

#if (BOARD != BEAGELEBONE_BLACK)
//-------------setReference-------------
/** @~english 
* @brief set Reference.
*
* @param ref 
* @return success: 1, failure: -1
*
* @~german 
* @brief setzt Referenz.
*
* @param ref 
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_adc::setReference(int ref){
	error_flag = false;
	return 1;
}
#endif


#endif

//*******************************************************************
//Class for accessing GNUBLIN serial interface
//*******************************************************************

//------------------local defines-----------------
/** @~english 
* @brief creates macro reference for default device "/dev/eser0"
*
* @~german 
* @brief definiert das Standard seriell device "/dev/eser0"
*
*/
#define BAUDRATE B9600
#define SERIALDEVICE "/dev/eser0"

//------------------Konstruktor------------------
/** @~english
* @brief initalizes the serial interface. Sets the devicefile to "/dev/eser0"
*
* @~german
* @brief initialisiert die serielle Schnittstelle. Setzt das standard serielle device auf "/dev/eser0"
*
*/
gnublin_serial::gnublin_serial() 
{
	init(SERIALDEVICE, BAUDRATE);
}

//------------------Konstruktor------------------
/** @~english
* @brief initalizes the serial interface. Sets the devicefile to "/dev/eser0"
*
* @~german
* @brief initialisiert die serielle Schnittstelle. Setzt das standard serielle device auf "/dev/eser0"
*
*/
gnublin_serial::gnublin_serial(std::string Devicefile) 
{
	init(Devicefile, BAUDRATE);
}

//------------------Konstruktor------------------
/** @~english
* @brief initalizes the serial interface. Sets the devicefile to "/dev/eser0"
*
* @~german
* @brief initialisiert die serielle Schnittstelle. Setzt das standard serielle device auf "/dev/eser0"
*
*/
gnublin_serial::gnublin_serial(std::string Devicefile, int rate) 
{
	init(Devicefile, rate);
}

//------------------init------------------
/** @~english
* @brief Called by the constructors to initialize class variables.
* @param Devicefile new serial device file, e.g. "/dev/eser1"
* @param rate new Baudrate e.g. 38400
*
* @~german
* @brief Wird von den Konstruktoren der Klasse aufgerufen um die Variablen zu initialisieren.
* @param Devicefile neues serial device file, z.B. "/dev/eser1"
* @param rate Neue Baudrate z.B. 38400
*
*/
void gnublin_serial::init(std::string Devicefile, int rate) 
{
	devicefile=Devicefile;
	baudrate=rate;
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
int gnublin_serial::errorMsg(std::string message)
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
void gnublin_serial::close_fd()
{
	if (fd) {
		close(fd);
		fd = 0;
	}
}

//------------------open file descriptor------------------
/** @~english
* @brief Opens the devicefile. If a file is already open it is closed first.  A new file is opened
* and settings based on the class values for devicefile and baudrate were made
*
* @return success: 0, failure: -1
*
* @~german
* @brief Öffnet die Geräte Datei. Wenn eine Datei bereits geöffnet ist, wird sie zunächst geschlossen. 
* Eine neue Datei wird geöffnet und Einstellungen vorgenommen, basierend auf den Klassenvariablen devicefile und Baudrate
*
* @return Erfolg: 0, Misserfolg: -1
*
*/
int gnublin_serial::open_fd() 
{
	error_flag = false;

	if (fd) { 
		close_fd(); 
		fd = 0;
	}

	if ((fd = open(devicefile.c_str(), O_RDWR | O_NOCTTY | O_NDELAY)) < 0) 
		return errorMsg("ERROR opening: " + devicefile + "\n");

	if(!isatty(fd)) { errorMsg("Error isatty(fd)\n"); }
	if(tcgetattr(fd, &config) < 0) { errorMsg("Error tcgetattr\n"); }
	//
	// Input flags - Turn off input processing
	// convert break to null byte, no CR to NL translation,
	// no NL to CR translation, don't mark parity errors or breaks
	// no input parity check, don't strip high bit off,
	// no XON/XOFF software flow control
	//
	config.c_iflag &= ~(IGNBRK | BRKINT | ICRNL |
		            INLCR | PARMRK | INPCK | ISTRIP | IXON);
	//
	// Output flags - Turn off output processing
	// no CR to NL translation, no NL to CR-NL translation,
	// no NL to CR translation, no column 0 CR suppression,
	// no Ctrl-D suppression, no fill characters, no case mapping,
	// no local output processing
	//
	// config.c_oflag &= ~(OCRNL | ONLCR | ONLRET |
	//                     ONOCR | ONOEOT| OFILL | OLCUC | OPOST);
	config.c_oflag = 0;
	//
	// No line processing:
	// echo off, echo newline off, canonical mode off, 
	// extended input processing off, signal chars off
	//
	config.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
	//
	// Turn off character processing
	// clear current char size mask, no parity checking,
	// no output processing, force 8 bit input
	//
	config.c_cflag &= ~(CSIZE | PARENB);
	config.c_cflag |= CS8;
	//
	// One input byte is enough to return from read()
	// Inter-character timer off
	//
	config.c_cc[VMIN]  = 1;
	config.c_cc[VTIME] = 0;
	//
	// Communication speed (simple version, using the predefined
	// constants)
	//
	switch(baudrate){
		case 300:
			if(cfsetispeed(&config, B300) < 0 || cfsetospeed(&config, B300) < 0) {
			    printf("Error set I/O Speed\n");
			}
		break;
		case 600:
			if(cfsetispeed(&config, B600) < 0 || cfsetospeed(&config, B600) < 0) {
			    printf("Error set I/O Speed\n");
			}
		break;
		case 1200:
			if(cfsetispeed(&config, B1200) < 0 || cfsetospeed(&config, B1200) < 0) {
			    printf("Error set I/O Speed\n");
			}
		break;
		case 2400:
			if(cfsetispeed(&config, B2400) < 0 || cfsetospeed(&config, B2400) < 0) {
			    printf("Error set I/O Speed\n");
			}
		break;	
		case 4800:
			if(cfsetispeed(&config, B4800) < 0 || cfsetospeed(&config, B4800) < 0) {
			    printf("Error set I/O Speed\n");
			}
		break;
		case 9600:
			if(cfsetispeed(&config, B9600) < 0 || cfsetospeed(&config, B9600) < 0) {
			    printf("Error set I/O Speed\n");
			}
		break;
		case 19200:
			if(cfsetispeed(&config, B19200) < 0 || cfsetospeed(&config, B19200) < 0) {
			    printf("Error set I/O Speed\n");
			}
		break;
		case 38400:
			if(cfsetispeed(&config, B38400) < 0 || cfsetospeed(&config, B38400) < 0) {
			    printf("Error set I/O Speed\n");
			}
		break;
/*
		case 76800:
			if(cfsetispeed(&config, B76800) < 0 || cfsetospeed(&config, B76800) < 0) {
			    printf("Error set I/O Speed\n");
			}
		break;
*/
		case 115200:
			if(cfsetispeed(&config, B115200) < 0 || cfsetospeed(&config, B115200) < 0) {
			    printf("Error set I/O Speed\n");
			}
		break;
/*		case 153600:
			if(cfsetispeed(&config, B153600) < 0 || cfsetospeed(&config, B153600) < 0) {
			    printf("Error set I/O Speed\n");
			}
		break;
		case 307200:
			if(cfsetispeed(&config, B307200) < 0 || cfsetospeed(&config, B307200) < 0) {
			    printf("Error set I/O Speed\n");
			}
		break;
		case 614400:
			if(cfsetispeed(&config, B614400) < 0 || cfsetospeed(&config, B614400) < 0) {
			    printf("Error set I/O Speed\n");
			}
		break;
		case 1228800:
			if(cfsetispeed(&config, B1228800) < 0 || cfsetospeed(&config, B1228800) < 0) {
			    printf("Error set I/O Speed\n");
			}
		break;
*/
	}
	//
	// Finally, apply the configuration
	//
	if(tcsetattr(fd, TCSAFLUSH, &config) < 0) {  printf("Error set Attr\n"); }

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
bool gnublin_serial::fail(){
	return error_flag;
}

//-------------------set devicefile----------------
/** @~english
* @brief set the serial device file. default is "/dev/eser0"
*
* This function sets the devicefile you want to access. by default "/dev/eser0" is set.
* @param filename path to the devicefile e.g. "/dev/eser0"
* @return failure: -1 
*
* @~german
* @brief setzt die Serielle Device Datei. Standard ist die "/dev/eser0"
*
* Diese Funktion setzt die Geräte Datei, auf die man zugreifen möchte. Standardmäßig ist bereits "/dev/eser0" gesetzt.
* @param filename Dateipfad zur Geräte Datei, z.B. "/dev/eser0"
* @return failure: -1 
*/
int gnublin_serial::setDevicefile(std::string filename){
	devicefile = filename;
	return open_fd();
}

//-------------------set baudrate----------------
/** @~english
* @brief sets the baudrate
*
* This function sets the baudrate you want. supported baudrates are: 300, 600, 1200, 2400, 4800, 9600, 19200, 38400, 76800, 115200, 153600, 307200, 614400 and 1228800
* @param rate Baudrate
* @return failure: -1 
*
* @~german
* @brief setzt die I2C Device Datei. Standard ist die "/dev/i2c-1"
*
* Diese Funktion setzt die Baudrate. Es werden folgende Baudraten unterstüzt: 300, 600, 1200, 2400, 4800, 9600, 19200, 38400, 76800, 115200, 153600, 307200, 614400 und 1228800
* @param rate Baudrate
* @return failure: -1 
*/
int gnublin_serial::setBaudrate(int rate){
	baudrate=rate;
	return open_fd();
}

//----------------------------------send----------------------------------
/** @~english 
* @brief send bytes to the serial Interface
*
* This function sends "length" number of bytes from the "TxBuf" to the serial interface. At success the function returns 1, on failure -1.<br>
* e.g.<br>
* send 2 bytes from buf to the serial interface<br>
* send (buf, 2);
* @param TxBuf Transmit buffer. The bytes you want to send are stored in it.
* @param length Amount of bytes that will be send.
* @return success: 1, failure: -1
*
* @~german 
* @brief sendet Bytes an den serielle Schnittstelle.
*
* Diese Funktion sendet "length" Anzahl an Bytes aus dem "TxBuf" an die serielle Schnittstelle. Bei Erfolg wird 1 zurück gegeben, bei Misserfolg -1.<br>
* Beispiele:<br>
* Sende 2 Bytes von "buf" an den i2c Bus:
* send(buf, 2);
* @param RxBuf Sende Puffer. Die zu sendenden Bytes sind hier gespeichert.
* @param length Anzahl der zu sendenden Bytes.
* @return Erfolg: 1, Misserfolg: -1
*/
int gnublin_serial::send(unsigned char *TxBuf, int length){

	if (TxBuf == 0)
		return errorMsg("Send method received a null TxBuf pointer.\n");
	if (length < 1)
		return errorMsg("Send method received an invalid buffer length.\n");

	if (!fd)
		if (open_fd() == -1)
			  return -1;

	error_flag=false;	

	if(write(fd, TxBuf, length) != length)
		return errorMsg("serial write error!\n");

	return 1;
}


//***************************************************************************
// Class for creating pwm signals
//***************************************************************************


/**
* @~english
* @brief Default clock is clk1 with 1400 Hz.
*
* Clock divider:
* clk1: clock divider off
* clk2: half clock
* clk3: quarter clock
* clk4: eighth clock
* @~german
* @brief Standard Clock ist clk1i.
*
* Clock-Teiler:
* clk1: Clock-Teiler aus
* clk2: Halbe Clock
* clk3: Viertel Clock
* clk4: Achtel Clock
*/


//******************** constructor ******************************************
/**
* @~english
* @brief Constructs the object.
*
* @~german
* @brief Konstruiert das Objekt.
*
*/

gnublin_pwm::gnublin_pwm() {  }		// ctor



//********************** setValue **********************************************
/**
 *
 * @~english
 * @brief Specifies the Duty-Cycle. 0 would be equal to LOW and 100 would be equal to HIGH.
 *
 *      
 * @param Float vlaue from 0 to 100. Other values will be ignored.
 *
 * @~german
 * @brief Definiert den Duty-Cycle. 0 wäre wie ein logisches LOW und 100 ein logisches HIGH.
 *
 * @param Float Wert von 0 bis 100. Andere Werte werden ignoriert.
 *
 */

void gnublin_pwm::setValue(float v) {
 	int pwm_raw = 0;
	pwm_raw = int(v/100 * 4095);
	
	if(pwm_raw < 0 || pwm_raw > 4095) {
		printf("Enter a value between 0 and 100");
		return;
	}
	// Convert to hex
	stringstream stream;
	stream << setfill ('0') << setw(sizeof(char)*3) << hex << pwm_raw;
	string pwm_hex( stream.str() );
	
	cout << pwm_hex << endl;	//debug line
	
	std::ofstream pwm_file ("/dev/lpc313x_pwm");
	pwm_file << pwm_hex;	
	return;
}



//********************** setClock *************************************
/**
 *
 * @~english
 * @brief Specifies the Clock divider
 *      
 * @param clock number (integer). 
 * 1: clk1, no clock divider, 1400 Hz 
 * 2: clk2, half clock
 * 3: clk3, quarter clock
 * 4: clk4, eighth clock
 *
 * @~german
 * @brief Definiert den Clock-Teiler
 *
 * @param delimiterSign Zeichen, welches als Trennsymbol genutzt werden soll 
 * 1: clk1, Clock-Teiler aus, 1400 Hz
 * 2: clk2, Halbe Clock
 * 3: clk3, Viertel Clock
 * 4: clk4, Achtel Clock
 *
 */

void gnublin_pwm::setClock(int num) {
	string clock_str;

	switch (num) {
		case 1:		clock_str = "clk1";
							break;
		case 2:		clock_str = "clk2";
							break;
		case 3:		clock_str = "clk3";
							break;
		case 4: 	clock_str = "clk4";
							break;
		default:	clock_str = "clk1";
							break;
	}

	ofstream pwm_file ("/dev/lpc313x_pwm");
	pwm_file << clock_str;
	return;
}



//***************************************************************************
// Class for accesing the GNUBLIN MODULE-DISPLAY 2x16
//***************************************************************************


//********** Constructor ***************************

/**
* @~english
* @brief Set the default RS-PIN
*
* Default RS-Pin:
* GNUBLIN: GPIO14
* RASPBERRY PI: GPIO4
* BEAGLEBONE_BLACK: GPIO60
*
* @~german
* @brief Setzt die standard RS-Pins
*
* Standard RS-Pin:
* GNUBLIN: GPIO14
* RASPBERRY PI: GPIO4
* BEAGLEBONE_BLACK: GPIO60
*/
gnublin_module_dogm::gnublin_module_dogm(){

#if (BOARD == RASPBERRY_PI)
	rs_pin = 4;
#elif (BOARD == BEAGLEBONE_BLACK)
	rs_pin = 60;
#else
	rs_pin = 14;
#endif
	gpio.pinMode(rs_pin, OUTPUT);
	error_flag = false;
	spi.setSpeed(100000);
	spi.setLength(8);	
}

//********* init()**********************************
// execute initialisation of the display
// parameters: 	NONE
// return: 	* [int] 1	for success
//		* [int] -1  	for failure

/**
* @~english
* @brief Initialisizes the Display
*
* @return 1 by success, -1 by failure
*
* @~german
* @brief Initialisiert das Display
*
* @return 1 bei Erfolg, -1 im Fehlerfall
*/
int gnublin_module_dogm::init(){
	__u8 init_str[] = {0x39, 0x14, 0x55, 0x6D, 0x78, 0x38, 0x0C, 0x01, 0x06};
	if (gpio.digitalWrite(rs_pin, LOW) <0){
			error_flag = true;
			return -1;
	}
	if (spi.send(init_str, 9) < 0){
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return 1;
}




//************ fail() *******************************

/**
* @~english
* @brief Returns the errorflag to detect error in the previous called method.
*
* @return error_flag
*
* @~german
* @brief Gibt das errorflag zurück, um Fehler in der zuvor aufgerugfenen Methode zu erkennen.
*
* @return error_flag
*/
bool gnublin_module_dogm::fail(){
	return error_flag;
}


//-------------get Error Message-------------
/**
* @~english
* @brief Returns the ErrorMessage of the previous error if one exist.
*
* @return ErrorMessage as C-String
*
* @~german
* @brief Gibt die Fehlernachricht des zuvor aufgetretenen Fehlers zurück, wenn weine exisitert.
*
* @return ErrorMessage als C-String
*/
const char *gnublin_module_dogm::getErrorMessage(){
	return ErrorMessage.c_str();
}


//*********** setRsPin() *****************************

/**
* @~english
* @brief Set the custom RS pin
*
* @param pin Number of the RS-pin
* @return 1 by success, -1 by failure
*
* @~german
* @brief Setzt den benutzerdefinierten RS-Pin.
*
* @param pin Nummer des RS-Pin
* @return 1 bei Erfolg, -1 im Fehlerfall
*/
int gnublin_module_dogm::setRsPin(int pin){
	rs_pin = pin;
	gpio.pinMode(rs_pin, OUTPUT);
	return 1;
}


//*********** setCS ************************************

/**
* @~english
* @brief Set the custom chipselect pin
*
* @param cs Number of the chipselect-pin
* @return 1 by success, -1 by failure
*
* @~german
* @brief Setzt den benutzerdefinierten Chipselect-Pin.
*
* @param cs Nummer des Chipselect-Pin
* @return 1 bei Erfolg, -1 im Fehlerfall
*/
int gnublin_module_dogm::setCS(int cs){
	if (spi.setCS(cs) < 0){
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return 1;
}


//************ print() **********************************

/**
* @~english
* @brief Print string on display
*
* @param buffer buffor of the string
* @return 1 by success, -1 by failure
*
* @~german
* @brief Schreibt String auf das Display
*
* @param buffer Buffer für den String
* @return 1 bei Erfolg, -1 im Fehlerfall
*/
int gnublin_module_dogm::print(char* buffer){
	__u8 tmp[32];
	int len = strlen(buffer);
	for(int i=0; i<len; i++){
		tmp[i] = buffer[i];
	}
	gpio.digitalWrite(rs_pin, HIGH);
	if (spi.send(tmp, len) < 0){
		gpio.digitalWrite(rs_pin, LOW);
		error_flag = true;
		return -1;
	}
	gpio.digitalWrite(rs_pin, LOW);
	error_flag = false;
	return 1;
}


//************ print() **********************************

/**
* @~english
* @brief Print string on a specific line of the display
*
* @param buffer buffor of the string
* @param line number of the line (1,2)
* @return 1 by success, -1 by failure
*
* @~german
* @brief Schreibt String in eine bestimmte Zeile des Displays
*
* @param buffer Buffer für den String
* @param line Zeilenenummer (1,2)
* @return 1 bei Erfolg, -1 im Fehlerfall
*/
int gnublin_module_dogm::print(char* buffer, int line){
	error_flag = false;
	if (returnHome() < 0){
		return -1;
	}
	if (line == 1){
		print(buffer);
	}
	else if (line == 2) {
		offset(16);
		print(buffer);
	}
	else {
		error_flag = true;
		return -1;
	}
	return -1;
}


//************ print() **********************************

/**
* @~english
* @brief Print string on a specific line with given offset on the display
*
* @param buffer buffor of the string
* @param line number of the line (1,2)
* @param off number of elements for the offset
* @return 1 by success, -1 by failure
*
* @~german
* @brief Schreibt String in eine bestimmte Zeile mit gegebenem Offset auf das Displays
*
* @param buffer Buffer für den String
* @param line Zeilenenummer (1,2)
* @param off Anzahl der Positionen, um die verschoben werden soll
* @return 1 bei Erfolg, -1 im Fehlerfall
*/
int gnublin_module_dogm::print(char* buffer, int line, int off){
	error_flag = false;
	returnHome();
	if (line == 1){
		offset(off);
		print(buffer);
	}
	else if (line == 2) {
		off +=16;
		offset(off);
		print(buffer);
	}
	else {
		error_flag = true;
		return -1;
	}
	return -1;
}


//****************** offset() **********************************

/**
* @~english
* @brief Set an offset to the display
*
* @param num Number of the element on which the cursor will be set
* @return 1 by success, -1 by failure
*
* @~german
* @brief Setzt den Cursor an eine bestimmte Position
*
* @param num Nummber der Zeichnposition an die der Cursor gesetzt werden soll
* @return 1 bei Erfolg, -1 im Fehlerfall
*/
int gnublin_module_dogm::offset(int num){
	__u8 tmp;
	if (num >= 0 && num < 16){
		tmp = num + 128;
	}
	else if (num >= 16 && num < 32){
		tmp = num - 16 + 192;
	}
	else {
		error_flag = true;
		return -1;
	}
	if (spi.send(&tmp, 1) < 0){
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return 1;
}


//*************** clear() ****************************************

/**
* @~english
* @brief clear the display
*
* @return 1 by success, -1 by failure
*
* @~german
* @brief Löscht den Inhalt des Displays.
*
* @return 1 bei Erfolg, -1 im Fehlerfall
*/
int gnublin_module_dogm::clear(){
	__u8 clear_cmd = 0x01;
	if (spi.send(&clear_cmd, 1) < 0){
		error_flag = true;
		return -1;
	}
	return 1;
}


//**************** returnHome() ************************************

/**
* @~english
* @brief Reset the Display
*
* @return 1 by success, -1 by failure
*
* @~german
* @brief Resetet das Display.
*
* @return 1 bei Erfolg, -1 im Fehlerfall
*/
int gnublin_module_dogm::returnHome(){
	__u8 return_cmd = 0x02;
	if (spi.send(&return_cmd, 1) < 0){
		error_flag = true;
		return -1;
	}
	return 1;
}	


//**************** shift() *****************************************

/**
* @~english
* @brief Shift the whole display
*
* @param num Number of shifts, positive: right, negativ: left
* @return 1 by success, -1 by failure
*
* @~german
* @brief Verschiebt das ganze Display
*
* @param num Anzahl der zu verschiebenden Positionen, positiv: rechts, negativ: left
* @return 1 bei Erfolg, -1 im Fehlerfall
*/
int gnublin_module_dogm::shift(int num){
	__u8 shift_cmd;
	if (num > 0){
		shift_cmd = 0x1C;
		for (int i=0; i < num; i++){
			if(spi.send(&shift_cmd, 1) < 0){
				error_flag = true;
				return -1;
			}
		}
	}
	else if (num < 0){
		shift_cmd = 0x18;
		for (int i=0; i > num; i--){
			if(spi.send(&shift_cmd, 1) < 0){
				error_flag = true;
				return -1;
			}
		}
	}
	else {
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return 1;
}


//*********************** controlDisplay() **********************************

/**
* @~english
* @brief Set display parameters
*
* @param power switch display on(1) or off(0)  (not the chontrollerchip)
* @param cursor switch cursor on(1) or off(0)
* @param blink switch the vlinking of the cursor on(1) or off(0)
* @return 1 by success, -1 by failure
*
* @~german
* @brief Setzt Displayparameter.
*
* @param power Display an(1) oder aus(0) schalten (nicht den Displaycontroller)
* @param cursor Schatet den Cursor an(1) oder aus(0)
* @param blink Schaltet das Blinken des Cursors an(1) oder aus(0)
* @return 1 bei Erfolg, -1 im Fehlerfall
*/
int gnublin_module_dogm::controlDisplay(int power, int cursor, int blink) {
	__u8 display_cmd = 0x08;
	if (power == 1) {
		display_cmd += 0x04;
	}
	if (cursor == 1) {
		display_cmd += 0x02;
	}
	if (blink == 1) {
		display_cmd += 0x01;
	}
	if (spi.send(&display_cmd, 1) < 0){
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return 1;
}
//*******************************************************************
//Class for accessing the LM75 IC via I2C
//*******************************************************************

//------------------Konstruktor------------------
/** @~english 
* @brief Sets the error_flag to "false", the closemode to "1" (see i2c for details) and the standard i2c Address to 0x4f
*
* @~german 
* @brief Setzt das error_flag auf "false", den closemode auf "1" (siehe i2c für Details) und die Standard i2c Adresse auf 0x4f
*
*/
gnublin_module_lm75::gnublin_module_lm75()
{
	error_flag=false;
	setAddress(0x4f);
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
const char *gnublin_module_lm75::getErrorMessage(){
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
bool gnublin_module_lm75::fail(){
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
void gnublin_module_lm75::setAddress(int Address){
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
void gnublin_module_lm75::setDevicefile(std::string filename){
	i2c.setDevicefile(filename);
}


//-----------------------------------get Temp-----------------------------------
/** @~english
* @brief reads the raw data via i2c from the LM75 chip and calculates the temperature
*
* @return Returns the Temperature, 0 at failure (check with fail() and getErrorMessage())
*
* @~german
* @brief liest die Roh-Daten aus dem LM75 und berechnet die Temperatur
*
* @return Temperatur, im Fehlerfall 0 (überprüfen mit fail() und getErrorMessage())
*/
int gnublin_module_lm75::getTemp(){
	short value=0;
	int temp;	
	unsigned char rx_buf[2];
	error_flag=false;

	if(i2c.receive(0x00, rx_buf, 2)>0){
		// rx_buf[0] = MSByte
		// rx_buf[1] = LSByte
		// save the MSB
		value = rx_buf[0];
		// make space for the LSB
		value<<=8;
		// save the LSB
		value |= rx_buf[1];
		// Bit 0-4 isn't used in the LM75, so shift right 5 times
		value>>=5;

		//check if temperature is negative
		if(rx_buf[0] & 0x80)
		{
			value = value|0xF800;
			value =~value +1;
			temp = value*(-0.125);
			return temp;
		}
		else { //temperature is positive
			temp = value*0.125;
			return temp;
		}


	}
	else{
		error_flag=true;
		ErrorMessage = "Error i2c receive\n";
		return 0;	
	}
}


//--------------------------------get Temp float--------------------------------
/** @~english
* @brief reads the raw data via i2c from the LM75 chip and calculates the temperature
*
* @return Returns the Temperature as float, 0 at failure (check with fail() and getErrorMessage())
*
* @~german
* @brief liest die Roh-Daten aus dem LM75 und berechnet die Temperatur
*
* @return Temperatur als Fließkommazahl, im Fehlerfall 0 (überprüfen mit fail() und getErrorMessage())
*/
float gnublin_module_lm75::getTempFloat(){
	short value=0;
	float temp;	
	unsigned char rx_buf[2];
	error_flag=false;

	if(i2c.receive(0x00, rx_buf, 2)>0){
		// rx_buf[0] = MSByte
		// rx_buf[1] = LSByte
		// save the MSB
		value = rx_buf[0];
		// make space for the LSB
		value<<=8;
		// save the LSB
		value |= rx_buf[1];
		// Bit 0-4 isn't used in the LM75, so shift right 5 times
		value>>=5;

		//check if temperature is negative
		if(rx_buf[0] & 0x80)
		{
			value = value|0xF800;
			value =~value +1;
			temp = value*(-0.125);
			return temp;
		}
		else { //temperature is positive
			temp = value*0.125;
			return temp;
		}
		
	}
	else{	
		error_flag=true;
		ErrorMessage="Error i2c receive\n";
		return 0;
	}
		
}


//--------------------------------get Value---------------------------------
/** @~english
* @brief reads the raw data via i2c from the LM75 chip and shift the bits correctly
*
* @return Returns raw value, already shifted. 0 at failure (check with fail() and getErrorMessage())
*
* @~german
* @brief liest die Roh-Daten aus dem LM75 und schiebt die Bits in die richtige Reihenfolge
*
* @return Rohwert, im Fehlerfall 0 (überprüfen mit fail() und getErrorMessage())
*/
short gnublin_module_lm75::getValue(){
	short value=0;
	unsigned char rx_buf[2];
	error_flag=false;
	if(i2c.receive(0x00, rx_buf, 2)>0){
		
		// rx_buf[0] = MSByte
		// rx_buf[1] = LSByte
		// save the MSB
		value = rx_buf[0];
		// make space for the LSB
		value<<=8;
		// save the LSB
		value |= rx_buf[1];
		// Bit 0-4 isn't used in the LM75, so shift right 5 times
		value>>=5;
		return value;
		
	}
	else{
		error_flag=true;
		ErrorMessage="Error i2c receive\n";
		return 0;
	}
}


//*****************************************************************************
// Class for accesing GNUBLIN Module-ADC / ADS7830
//*****************************************************************************

/**
* @~english
* @brief Set the default I2C Address and reference to intern
*
* Default I2C Address: 0x48
*
* @~german
* @brief Setzt die standard I2C Adresse und die Referenzspannung auf intern
*
* Standard I2C Addresse: 0x48
*/
gnublin_module_adc::gnublin_module_adc() {
	i2c.setAddress(0x48);
	referenceValue = 2500;
	reference_flag = IN;
	error_flag = false;
}

//-------------get Error Message-------------

/**
* @~english
* @brief Returns the ErrorMessage of the previous error if one exist.
*
* @return ErrorMessage as C-String
*
* @~german
* @brief Gibt die Fehlernachricht des zuvor aufgetretenen Fehlers zurück, wenn weine exisitert.
*
* @return ErrorMessage als C-String
*/
const char *gnublin_module_adc::getErrorMessage(){
	return ErrorMessage.c_str();
}


//------------ fail() -----------------------

/**
* @~english
* @brief Returns the errorflag to detect error in the previous called method.
*
* @return error_flag
*
* @~german
* @brief Gibt das errorflag zurück, um Fehler in der zuvor aufgerugfenen Methode zu erkennen.
*
* @return error_flag
*/
bool gnublin_module_adc::fail() {
	return error_flag;
}


//-------------setAddress-------------
// set the slave address
// parameters:		[int]address	i2c slave Address
// return:		NONE
/**
* @~english
* @brief Set the I2C Address
*
* @param address I2C Address
* @return 1 by success, -1 by failure
*
* @~german
* @brief Setzt die I2C Adresse.
*
* @param address I2C Adresse
* @return 1 bei Erfolg, -1 im Fehlerfall
*/
int gnublin_module_adc::setAddress(int address) {
	i2c.setAddress(address);
	if (i2c.fail()) {
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return 1;
}


//-------------------setDevicefile----------------
// set the i2c device file. default is "/dev/i2c-1"
// parameters:		[string]filename	path to the dev file
// return:		NONE
/**
* @~english
* @brief Set a custom I2C devicefile
*
* default devicefile: "/dev/i2c-1"
* @param filename path of the devicefile
* @return 1 by success, -1 by failure
*
* @~german
* @brief Setzt ein benutzerdefiniertes Devicefile.
*
* Standard Devicefile ist "/dev/i2c-1"
* @param filename Pfad zum Devicefile
* @return 1 bei Erfolg, -1 im Fehlerfall
*/
int gnublin_module_adc::setDevicefile(std::string filename) {
	i2c.setDevicefile(filename);
	if (i2c.fail()) {
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return 1;
}

//-------------------setReference() ----------------

/**
* @~english
* @brief Set the reverencevoltage to intern or extern
*
* @param value IN (1) for intern (2.5V), OUT (0) for extern (3.3V)
* @return 1 by success, -1 by failure
*
* @~german
* @brief Setzt die referenzspannung auf intern oder extern.
*
* @param value IN (1) für intern (2,5V), OUT (0) für extern (3,3V)
* @return 1 bei Erfolg, -1 im Fehlerfall
*/
int gnublin_module_adc::setReference(int value) {
	if (value == 0) {
		referenceValue = 3300;
		reference_flag = 0;
	}
	else if (value == 1) {
		referenceValue = 2500;
		reference_flag = 1;
	}
	else {
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return 1;
}


//---------------------- getValue() -----------------------

/**
* @~english
* @brief Get a value of an ADC channel in reference to GND
*
* @param channel Number of the ADC-channel (1-8)
* @return value
*
* @~german
* @brief Liefert den Wert eine ADC Ports bezogen zu GND
*
* @param channel Nummer des ADC-Ports (1-8)
* @return Wert
*/
int gnublin_module_adc::getValue(int channel) {
	int command;
	unsigned char value[1];
	
	if (reference_flag == 0){
		switch (channel) {
			case 1: command = 0x87; break;
			case 2: command = 0xC7; break;
			case 3: command = 0x97; break;
			case 4: command = 0xD7; break;
			case 5: command = 0xA7; break;
			case 6: command = 0xE7; break;
			case 7: command = 0xB7; break;
			case 8: command = 0xF7; break;
			default: error_flag = true; return -1; break;
		}
	}
	else {
		switch (channel) {
			case 1: command = 0x8F; break;
			case 2: command = 0xCF; break;
			case 3: command = 0x9F; break;
			case 4: command = 0xDF; break;
			case 5: command = 0xAF; break;
			case 6: command = 0xEF; break;
			case 7: command = 0xBF; break;
			case 8: command = 0xFF; break;
			default: error_flag = true; return -1; break;
		}
	}
	
	i2c.send(command);
	if (i2c.fail()) {
		error_flag = true;
		return -1;
	}

	i2c.receive(value, 1);
	if (i2c.fail()) {
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return value[0];	
}

/**
* @~english
* @brief Get a value of an ADC channel in reference to a second ADC channel (differential)
*
* This combinations are possible<br>
* 1 - 2<br>
* 3 - 4<br>
* 5 - 6<br>
* 7 - 8<br>
* 2 - 1<br>
* 4 - 3<br>
* 6 - 5<br>
* 8 - 7<br>
* @param channel1 Number of the first ADC-channel 
* @param channel2 Number of the second ADC-channel
* @return value
*
* @~german
* @brief Liefert den Wert eines ADC Ports bezogen auf einen zweiten ADC Port (differentielle Messung)
*
* Dies Kombinationen sind möglich:<br>
* 1 - 2<br>
* 3 - 4<br>
* 5 - 6<br>
* 7 - 8<br>
* 2 - 1<br>
* 4 - 3<br>
* 6 - 5<br>
* 8 - 7<br>
* @param channel1 Nummer des ersten ADC-Ports
* @param channel2 Nummer des zweiten ADC-Ports
* @return Wert
*/
int gnublin_module_adc::getValue(int channel1, int channel2) {
	int command = -1;
	unsigned char value[1];
	
	for (int i = 1; i<9; i += 2 ) {
		if (channel1 == i && channel2 == (i+1)){
			switch(i) {
				case 1: command = 0x00; break;
				case 3: command = 0x10; break;
				case 5: command = 0x20; break;
				case 7: command = 0x30; break;
			}
		}
		if (channel2 == i && channel1 == (i+1)) {
			switch(i) {
				case 1: command = 0x40; break;
				case 3: command = 0x50; break;
				case 5: command = 0x60; break;
				case 7: command = 0x70; break;
			}
		}
	}
	if (command == -1){
		error_flag = true;
		return -1;
	}
	if (reference_flag == 0) {
		command += 0x7;
	}
	else {
		command += 0xF;
	}
	i2c.send(command);
	if (i2c.fail()) {
		error_flag = true;
		return -1;
	}

	i2c.receive(value, 1);
	if (i2c.fail()) {
		error_flag = true;
		return -1;
	}
	error_flag = false;
	return value[0];
}


//---------------------- getVoltage() -----------------------
// get ADC Value of channel in reference to GND
// parameters:		[int] channel		ADC-Port
// return:		[int] voltage		voltage of ADC-channel in mV
/**
* @~english
* @brief Get the voltage of an ADC channel in reference to GND in mV
*
* @param channel Number of the ADC-channel (1-8)
* @return value in mV
*
* @~german
* @brief Liefert den Wert eine ADC Ports bezogen zu GND in mV
*
* @param channel Nummer des ADC-Ports (1-8)
* @return Wert in mV
*/
int gnublin_module_adc::getVoltage(int channel) {
	error_flag = false;
	int voltage;
	int value = getValue(channel);
	if (error_flag) {
		return -1;
	}
	
	voltage = value * referenceValue / 255;
	return voltage;
}

/**
* @~english
* @brief Get the voltage of an ADC channel in reference to a second ADC channel in mV (differential)
*
* This combinations are possible<br>
* 1 - 2<br>
* 3 - 4<br>
* 5 - 6<br>
* 7 - 8<br>
* 2 - 1<br>
* 4 - 3<br>
* 6 - 5<br>
* 8 - 7<br>
* @param channel1 Number of the first ADC-channel 
* @param channel2 Number of the second ADC-channel
* @return voltage in mV
*
* @~german
* @brief Liefert den Wert eines ADC Ports bezogen auf einen zweiten ADC Port in mV (differentielle Messung)
*
* Dies Kombinationen sind möglich:<br>
* 1 - 2<br>
* 3 - 4<br>
* 5 - 6<br>
* 7 - 8<br>
* 2 - 1<br>
* 4 - 3<br>
* 6 - 5<br>
* 8 - 7<br>
* @param channel1 Nummer des ersten ADC-Ports
* @param channel2 Nummer des zweiten ADC-Ports
* @return Wert in mV
*/
int gnublin_module_adc::getVoltage(int channel1, int channel2) {
	error_flag = false;
	int voltage;
	int value = getValue(channel1, channel2);
	if (error_flag) {
		return -1;
	}
	
	voltage = value * referenceValue / 255;
	return voltage;
}

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



//****************************************************************************
// Class for easy use of the GNUBLIN Module-Relay
//****************************************************************************


//------------------Konstruktor------------------
/** @~english 
* @brief Set standards.
*
* Set standard i2c address 0x20, set ErrorFlag false
* @~german 
* @brief Setze Standartwerte.
*
* Setze standard i2c Adresse 0x20 und setze das ErrorFlag auf false.
*/
gnublin_module_relay::gnublin_module_relay() {
	error_flag=false;
	setAddress(0x20);
	i2c.setAddress(0x20);
}


//-------------getErrorMessage-------------
/** @~english 
* @brief Get the last Error Message.
*
* This Funktion returns the last Error Message, which occurred in that Class.
* @return ErrorMessage as c-string
*
* @~german 
* @brief Gibt die letzte Error Nachricht zurück.
*
* Diese Funktion gibt die Letzte Error Nachricht zurück, welche in dieser Klasse gespeichert wurde.
* @return ErrorMessage als c-string
*/
const char *gnublin_module_relay::getErrorMessage(){
	return ErrorMessage.c_str();
}

//-------------------------------fail-------------------------------
/** @~english 
* @brief Returns the error flag. 
*
* If something went wrong, the flag is true.
* @return bool error_flag
*
* @~german 
* @brief Gibt das Error Flag zurück.
*
* Falls das Error Flag in der Klasse gesetzt wurde, wird true zurück gegeben, anderenfalls false.
* @return bool error_flag
*/
bool gnublin_module_relay::fail(){
	return error_flag;
}

//-------------setAddress-------------
/** @~english 
* @brief Set the slave address 
*
* With this function you can set the individual I2C Slave-Address of the module.
* @param Address new I2C slave Address
*
* @~german 
* @brief Setzt Slave Adresse.
*
* Mit dieser Funktion kann die individuelle I2C Slave-Adresse des Moduls gesetzt werden.
* @param Address neue I2C slave Adresse
*/
void gnublin_module_relay::setAddress(int Address){
	pca9555.setAddress(Address);
}


//-------------setDevicefile-------------
/** @~english 
* @brief Set devicefile.
*
* With this function you can change the I2C device file. Default is "/dev/i2c-1"
* @param filename path to the I2C device file
*
* @~german 
* @brief Setzt Device Datei.
*
* Mit dieser Funktion kann die I2C Gerätedatei geändert werden. Standartmäßig wird "/dev/i2c-1" benutzt.
* @param filename Pfad zur I2C Gerätedatei
*/
void gnublin_module_relay::setDevicefile(std::string filename){
	pca9555.setDevicefile(filename);
}

//-------------------switch Pin----------------
/** @~english 
* @brief Switch pin.
*
* This function sets the given relay to the given value.
* @param pin Number of the relay to switch (1-8)
* @param value close (1) or open (0) the relay (ON, OFF)
* @return success: 1, failure: -1
*
* @~german 
* @brief Schalte Pin.
*
* Schalten der einzelnen Relays. 
* @param pin Nummer des anzusteuernden Relays (1-8)
* @param value schließen (1) oder öffnen (0) des Relays (ON, OFF)
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_relay::switchPin(int pin, int value) {
	error_flag=false;

	if (pin < 1 || pin > 8) {
		error_flag=true;
		ErrorMessage="pin is not between 1-8!\n";
		return -1;
	}
	if (pca9555.pinMode((pin-1), OUTPUT) < 0){
		error_flag=true;
		ErrorMessage=pca9555.getErrorMessage(); //"pca9555.pinMode failed! Address correct?\n";
		return -1;
	}
	if (pca9555.digitalWrite((pin-1), value) < 0) {
		error_flag=true;
		ErrorMessage="pca9555.digitalWrite failed! Address correct?\n";		
		return -1;
	}
	return 1;
}


//-----------------------------------readState-----------------------------------
/** @~english
* @brief reads the state of a pin and returns it
*
* With this function you can read the level of a pin.
* @param pin Number of the pin (0-15) you want to read from
* @return 0/1 logical level of the pin, failure: -1
*
* @~german
* @brief Liest den Zustand eins Pins und gibt ihn zurück
*
* Mit dieser Funktion kann man den Zustand eines Pins auslesen.
* @param pin Nummer des Pins (0-15) von dem man lesen will.
* @return 0/1 logischer Pegel des Pins, Misserfolg: -1
*/
int gnublin_module_relay::readState(int pin) {
	error_flag=false;
	unsigned char RxBuf[1];

	if (pin < 0 || pin > 15){
		error_flag=true;
		ErrorMessage="Pin Number is not between 0-15\n";
		return -1;
	}

	if(pin >= 0 && pin <= 7){ // Port 0		
		if(i2c.receive(0x02, RxBuf, 1)>0){

				RxBuf[0]<<=(8-pin); // MSB is now the pin you want to read from
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
	error_flag=true;
	ErrorMessage="something went wrong";
	return -1;
}





//*******************************************************************
//Class for accessing GNUBLIN Module-step
//*******************************************************************

//-------------gnublin_module_step-------------
/** @~english
* @brief Set irun and vmax to the default values (irun = 15, vmax = 8).
*
* @~german
* @brief Setzt die Standartwerte für irun und vmax (irun = 15, vmax = 8).
*
*/
gnublin_module_step::gnublin_module_step() {
  irun = 15;
  vmax = 8;
  stepmode = 12; // 1/16µStep mode
  accshape = 0;
}


//-------------getErrorMessage-------------
/** @~english
* @brief Get the last Error Message.
*
* This Funktion returns the last Error Message, which occurred in that Class.
* @return ErrorMessage as c-string
*
* @~german
* @brief Gibt die letzte Error Nachricht zurück.
*
* Diese Funktion gibt die Letzte Error Nachricht zurück, welche in dieser Klasse gespeichert wurde.
* @return ErrorMessage als c-string
*/
const char* gnublin_module_step::getErrorMessage() {
  return ErrorMessage.c_str();
}

//-------------setAddress-------------
/** @~english
* @brief Set the slave address
*
* With this function you can set the individual I2C Slave-Address of the module.
* @param Address new I2C slave Address
*
* @~german
* @brief Setzt Slave Adresse.
*
* Mit dieser Funktion kann die individuelle I2C Slave-Adresse des Moduls gesetzt werden.
* @param Address neue I2C slave Adresse
*/
void gnublin_module_step::setAddress(int Address) {
  i2c.setAddress(Address);
}

//-------------setDevicefile-------------
/** @~english
* @brief Set devicefile.
*
* With this function you can change the I2C device file. Default is "/dev/i2c-1"
* @param filename path to the I2C device file
*
* @~german
* @brief Setzt Device Datei.
*
* Mit dieser Funktion kann die I2C Gerätedatei geändert werden. Standartmäßig wird "/dev/i2c-1" benutzt.
* @param filename Pfad zur I2C Gerätedatei
*/
void gnublin_module_step::setDevicefile(std::string filename) {
  i2c.setDevicefile(filename);
}

//-------------setIrun-------------
/** @~english
* @brief Set Irun.
*
* This Funktion sets the new Irun value.
* @param newIrun values from 0 to 15 are possible
* @return success: 1, failure: -1
*
* @~german
* @brief Setze Irun.
*
* Diese Funktion setzt den neuen Irun Wert
* @param newIrun Werte von 0 bis 15
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_step::setIrun(unsigned int newIrun) {
  if(newIrun <= 15) {
    irun = newIrun;
    return 1;
  } else {
    return -1;
  }
}

//-------------setIhold-------------
/** @~english
* @brief Set Ihold.
*
* This Funktion sets the new Ihold value.
* @param newIhold values from 0 to 15 are possible
* @return success: 1, failure: -1
*
* @~german
* @brief Setze Ihold
*
* Diese Funktion setzt den neuen Ihold Wert
* @param newIhold Werte von 0 bis 15
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_step::setIhold(unsigned int newIhold) {
  if(newIhold <= 15) {
    ihold = newIhold;
    return 1;
  } else {
    return -1;
  }
}

//-------------setVmax-------------
/** @~english
* @brief Set Vmax.
*
* This Funktion sets the new Vmax value.
* @param newVmax values from 0 to 15 are possible
* @return success: 1, failure: -1
*
* @~german
* @brief Setze Vmax
*
* Diese Funktion setzt den neuen Vmax Wert
* @param newVmax Werte von 0 bis 15
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_step::setVmax(unsigned int newVmax) {
  if(newVmax <= 15) {
    vmax = newVmax;
    return 1;
  } else {
    return -1;
  }
}

//-------------setVmin-------------
/** @~english
* @brief Set Vmin.
*
* This Funktion sets the new Vmin value.
* @param newVmin values from 0 to 15 are possible
* @return success: 1, failure: -1
*
* @~german
* @brief Setze Vmin
*
* Diese Funktion setzt den neuen Vmin Wert
* @param newVmin Werte von 0 bis 15
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_step::setVmin(unsigned int newVmin) {
  if(newVmin <= 15) {
    vmin = newVmin;
    return 1;
  } else {
    return -1;
  }
}

//-------------setStepMode-------------
/** @~english
* @brief Set Step Mode.
*
* This Funktion sets the new StepMode value.
* @param newStepMode values: StepMode 0=Halfstep, 4=1/4µStep, 8=1/8µStep, 11=1/16µStep
* @return success: 1, failure: -1
*
* @~german
* @brief Setze Step Mode
*
* Diese Funktion setzt den neuen Step Mode Wert
* @param newStepMode Werte: StepMode 0=Halfstep, 4=1/4µStep, 8=1/8µStep, 11=1/16µStep
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_step::setStepMode(unsigned int newStepMode) {
  if((newStepMode == 0) || (newStepMode == 4) || (newStepMode == 8) || (newStepMode == 11)) {
    stepmode = newStepMode;
    return 1;
  } else {
    return -1;
  }
}

//-------------setAccShape-------------
/** @~english
* @brief Set Acc Shape.
*
* This Funktion sets the new AccShape value.
* @param newAccShape values: 0=AccShape off, 1=AccShape on
* @return success: 1, failure: -1
*
* @~german
* @brief Setze Acc Shape
*
* Diese Funktion setzt den neuen AccShape Wert
* @param newAccShape Werte: : 0=AccShape aus, 1=AccShape ein
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_step::setAccShape(unsigned int newAccShape) {
  if((newAccShape == 0) || (newAccShape == 1)) {
    accshape = newAccShape;
    return 1;
  } else {
    return -1;
  }
}

//-------------writeTMC-------------
/** @~english
* @brief Write to TMC.
*
* This funktion sends the amount of bytes (num) of the char array TxBuf to the TMC222 chip on the module-step.
* @param TxBuf Send buffer
* @param num amount of Bytes
* @return success: 1, failure: -1
*
* @~german
* @brief Sende an TMC.
*
* Diese Funktion sendet die Menge (num) an Bytes des Char Array TxBuf an den TMC222 Chip, welcher sich auf dem Moudule-step befindet.
* @param TxBuf Sende Puffer
* @param num Anzahl der zu sendenden Bytes
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_step::writeTMC(unsigned char* TxBuf, int num) {
  if(!i2c.send(TxBuf, num)) {
    return -1;
  } else {
    return 1;
  }
}

//-------------readTMC-------------
/** @~english
* @brief Read from TMC.
*
* This funktion reads the amount of bytes (num) of the TMC222 chip on the module-step and write it to the char array RxBuf.
* @param RxBuf Receive buffer
* @param num amount of Bytes
* @return success: 1, failure: -1
*
* @~german
* @brief Sende an TMC.
*
* Diese Funktion ließt die Menge (num) an Bytes vom TMC222 Chip, welcher sich auf dem Moudule-step befindet und übergibt das Ergebnis im Char Array RxBuf.
* @param RxBuf Empfangs Puffer
* @param num Anzahl der zu lesenden Bytes
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_step::readTMC(unsigned char* RxBuf, int num) {
  if(!i2c.receive(RxBuf, num)) {
    return -1;
  } else {
    return 1;
  }
}

//-------------burnNewAddress-------------
/** @~english
* @brief Burn a new I2C slave Address.
*
* This function calculates the needet bits to burn on the TMC222 chip, to get the new given slave-address. After that, the OTP bits are burned into the Chip.
* @param new_address the new slave address
* @return success: 1, failure: -1
*
* @~german
* @brief Brennt die übergebene slave Adresse.
*
* Diese Funktion berechnet die benötigten Bits, um die neue Adresse auf den Chip zu brennen. Anschließend werden die OTP bits gebrannt.
* Achtung: Sind die Bits einmal gesetzt, kann dies nicht wieder rückgängig gemacht werden. Falls eine Adresse eingegeben wird, die nicht möglich ist, wird dies von der Funktion gemeldet.
*
* @param new_address die neue Slave Adresse
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_step::burnNewAddress(int new_address) {

  //SetOTPParam
  unsigned char buffer[5];
  int new_ad = 0;
  int old_ad = 0;
  int slave_address = i2c.getAddress();
  std::string yes = "NO!";

  if(new_address <= slave_address) {
    printf("\tThe new address must be higher than the old one (0x%x)!\n", slave_address);
    return -1;
  } else if(new_address > 0x7f) {
    printf("The biggest slave address of the TMC222 is 0x7f\n");
    return -1;
  } else {
    old_ad = (slave_address & 0x1e) >> 1;
    new_ad = (new_address & 0x1e) >> 1;
    if(((new_ad & 0x1) < (old_ad & 0x1)) | ((new_ad & 0x2) < (old_ad & 0x2)) | ((new_ad & 0x4) < (old_ad & 0x4)) | ((new_ad & 0x8) < (old_ad & 0x8))) {
      printf("\tThis address could not be set, because the '1' cant be undone!\n"
             "\told OTP AD: 0x%x\n"
             "\tnew OTP AD: 0x%x\n", old_ad, new_ad);
      return -1;
    }
    if((new_address & 0x01) == 1) {
      printf("\tThe LSB address bit is set by the jumper on the module-step\n");
      new_address --;
      printf("\tThe new address will be set to: 0x%x \n", new_address);
    }

    printf("\tIf a bit of the OTP (on time programmable) is set, it cant be undone! \n\t"
           "If you are sure to write the new Address (0x%x) then type 'yes' in CAPITALS\n\n\t", new_address);
    std::cin >> yes;
    if(yes == "YES") {
      buffer[0] = 0x90; //SetOTPParam
      buffer[1] = 0xff; //N/A
      buffer[2] = 0xff; //N/A
      buffer[3] = 0x02; //set AD3 AD2 AD1 AD0
      buffer[4] = (unsigned char) new_ad;

      if(!i2c.send(buffer, 5)) {
        return -1;
      } else {
        printf("\tNew Address was successfully set to: 0x%x\n\tPlease replug the module.\n\n", new_address);
        return 1;
      }
    } else {
      printf("\tYou didn't type 'YES'\n");
      return -1;
    }
  }
}

//-------------getFullStatus-------------
/** @~english
* @brief Get full Status 1.
*
* This funktion sends the getFullStatus1 Command to the Chip.
* @return success: 1, failure: -1
*
* @~german
* @brief Get full Status 1.
*
* Diese Funktion sendet den getFullStatus1 Befehl an den Chip.
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_step::getFullStatus1() {
  if(i2c.send(0x81)) {
    return 1;
  } else {
    return -1;
  }
}

//-------------getFullStatus2-------------
/** @~english
* @brief Get full Status 2.
*
* This funktion sends the getFullStatus2 Command to the Chip.
* @return success: 1, failure: -1
*
* @~german
* @brief Get full Status 2.
*
* Diese Funktion sendet den getFullStatus2 Befehl an den Chip.
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_step::getFullStatus2() {
  if(i2c.send(0xfc)) {
    return 1;
  } else {
    return -1;
  }
}

//-------------runInit-------------
/** @~english
* @brief Run Init.
*
* This funktion sends the runInit Command to the Chip.
* @return success: 1, failure: -1
*
* @~german
* @brief Run Init.
*
* Diese Funktion sendet den runInit Befehl an den Chip.
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_step::runInit() {
  if(i2c.send(0x88)) {
    return 1;
  } else {
    return -1;
  }
}

//-------------setMotorParam-------------
/** @~english
* @brief Set motor parameter.
*
* This function sends the motor parameter to the module.
* @return success: 1, failure: -1
*
* @~german
* @brief Setze Motor Parameter.
*
* Diese Funktion sendet die eingestellten Motor Parameter an das Modul.
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_step::setMotorParam() {
  unsigned char buffer[8];
  //SetMotorParam
  buffer[0] = 0x89; //SetMotorParam
  buffer[1] = 0xff; //N/A
  buffer[2] = 0xff; //N/A
  buffer[3] = (unsigned char)((irun * 0x10) + ihold);  //Irun & I hold
  buffer[4] = (unsigned char)((vmax * 0x10) + vmin);  //Vmax & Vmin
  buffer[5] = 0x00; //status
  buffer[6] = 0x00; //securePos
  buffer[7] = (unsigned char)((accshape * 0x10) + stepmode);  //AccShape & StepMode 00=Halfstep, 04=1/4µStep, 08=1/8µStep, 0c=1/16µStep

  if(i2c.send(buffer, 8)) {
    return 1;
  } else {
    return -1;
  }
}

//-------------setMotorParam-------------
/** @~english
* @brief Set motor parameter.
*
* This function sends the motor parameter to the module.
* @param newIrun
* @param newIhold
* @param newVmax
* @param newVmin
* @param newAccShape
* @param newStepMode
* @return success: 1, failure: -1
*
* @~german
* @brief Setze Motor Parameter.
*
* Diese Funktion sendet die übergebenen Motor Parameter an das Modul.
* @param newIrun
* @param newIhold
* @param newVmax
* @param newVmin
* @param newAccShape
* @param newStepMode
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_step::setMotorParam(unsigned int newIrun, unsigned int newIhold, unsigned int newVmax, unsigned int newVmin, unsigned int newAccShape, unsigned int newStepMode) {
  irun = newIrun;
  ihold = newIhold;
  vmax = newVmax;
  vmin = newVmin;
  accshape = newAccShape;
  stepmode = newStepMode;

  unsigned char buffer[8];
  //SetMotorParam
  buffer[0] = 0x89; //SetMotorParam
  buffer[1] = 0xff; //N/A
  buffer[2] = 0xff; //N/A
  buffer[3] = (unsigned char)((irun * 0x10) + ihold);  //Irun & I hold
  buffer[4] = (unsigned char)((vmax * 0x10) + vmin);  //Vmax & Vmin
  buffer[5] = 0x00; //status
  buffer[6] = 0x00; //securePos
  buffer[7] = (unsigned char)((accshape * 0x10) + stepmode);  //AccShape & StepMode 00=Halfstep, 04=1/4µStep, 08=1/8µStep, 0c=1/16µStep

  if(i2c.send(buffer, 8)) {
    return 1;
  } else {
    return -1;
  }
}

//-------------hardStop-------------
/** @~english
* @brief Hard stop.
*
* This funktion sends the hardStop Command to the Chip. The motor will stop immediately.
* @return success: 1, failure: -1
*
* @~german
* @brief Hard Stop.
*
* Diese Funktion sendet den hardStop Befehl an den Chip. Der Motor bleibt sofort Stehen.
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_step::hardStop() {
  if(i2c.send(0x85)) {
    return 1;
  } else {
    return -1;
  }
}

//-------------softStop-------------
/** @~english
* @brief Soft stop.
*
* This funktion sends the softStop Command to the Chip. The motor slows down and stops.
* @return success: 1, failure: -1
*
* @~german
* @brief Soft Stop.
*
* Diese Funktion sendet den SoftStop Befehl an den Chip. Der Motor fährt langsam herrunter.
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_step::softStop() {
  if(i2c.send(0x8f)) {
    return 1;
  } else {
    return -1;
  }
}

//-------------resetPosition-------------
/** @~english
* @brief Reset Position.
*
* This funktion sends the resetPosition Command to the Chip. The internal position counter of the chip is reset to 0.
* @return success: 1, failure: -1
*
* @~german
* @brief Reset Position.
*
* Diese Funktion sendet den resetPosition Befehl an den Chip. Der interne Positions Zähler des Chips wird auf 0 zurückgesetzt.
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_step::resetPosition() {
  if(i2c.send(0x86)) {
    return 1;
  } else {
    return -1;
  }
}

//-------------setPosition-------------
/** @~english
* @brief Set Position.
*
* This funktion sends the Position Command to the Chip. The motor drives to the given position.
* @return success: 1, failure: -1
*
* @~german
* @brief Setze Position.
*
* Diese Funktion sendet den Position Befehl an den Chip. Der Motor fährt an die übergebene Position.
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_step::setPosition(int position) {
  unsigned char buffer[5];
  buffer[0] = 0x8B;   // SetPosition Command
  buffer[1] = 0xff;   // not avialable
  buffer[2] = 0xff;   // not avialable
  buffer[3] = (unsigned char)(position >> 8);   // PositionByte1 (15:8)
  buffer[4] = (unsigned char)  position;       // PositionByte2 (7:0)

  if(i2c.send(buffer, 5)) {
    return 1;
  } else {
    return -1;
  }
}

//-------------drive-------------
/** @~english
* @brief Drive.
*
* This Funktion reads the actual position from the motor and adds the amount of given steps to drive. So you can let the motor drive an amount of steps, without heaving trouble with the absolute positions.
* @return success: 1, failure: -1
*
* @~german
* @brief Fahre.
*
* Diese Funktion ließt die aktuelle Position des Motors und addiert die anzahl der übergebenen Schritte. So kann man den Motor einfach um eine bestimmte Anzahl Schritte fahren lassen, ohne sich über die absoulute Position gedanken machen zu müssen.
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_step::drive(int steps) {
  int old_position;
  int new_position;

  old_position = getActualPosition();
  new_position = old_position + steps;
  if(setPosition(new_position)) {
    return 1;
  } else {
    return -1;
  }
}

//-------------getMotionStatus-------------
/** @~english
* @brief Get motion status.
*
* This funktion Indicates the actual behavior of the position controller. <br>
* 0: Actual Position = Target Position; Velocity= 0 <br>
* 1: Positive Acceleration; Velocity > 0 <br>
* 2: Negative Acceleration; Velocity > 0 <br>
* 3: Acceleration = 0 Velocity = maximum pos Velocity <br>
* 4: Actual Position /= Target Position; Velocity= 0 <br>
* 5: Positive Acceleration; Velocity < 0 <br>
* 6: Positive Acceleration; Velocity < 0 <br>
* 7: Acceleration = 0 Velocity = maximum neg Velocity <br>
* -1: failure
*
* @return motionStatus
*
* @~german
* @brief Bewegungs-Status.
*
* Diese Funktion gibt den aktuellen bewegungs Status des module-step zurück. <br>
* 0: Aktuelle Position = Ziehl Position; Geschwindigkeit= 0 <br>
* 1: Positive Beschleunigung; Geschwindigkeit > 0 <br>
* 2: Negative Beschleunigung; Geschwindigkeit > 0 <br>
* 3: Beschleunigung = 0; Geschwindigkeit = maximum pos Geschwindigkeit <br>
* 4: Aktuelle Position /= Ziehl Position; Geschwindigkeit= 0 <br>
* 5: Positive Beschleunigung; Geschwindigkeit < 0 <br>
* 6: Positive Beschleunigung; Geschwindigkeit < 0 <br>
* 7: Acceleration = 0 Geschwindigkeit = maximum neg Geschwindigkeit  <br>
* -1: Fehler
* @return motionStatus
*/
int gnublin_module_step::getMotionStatus() {
  unsigned char RxBuf[8];
  int motionStatus = -1;
  if(getFullStatus1() == -1) {
    return -1;
  }


  if(!i2c.receive(RxBuf, 8)) {
    return -1;
  }
  motionStatus = (RxBuf[5] & 0xe0) >> 5;
  return motionStatus;

}


//-------------------get Switch status----------------
/** @~english
* @brief Get switch condition.
*
* This function checks the status of the connected switch and returns the value. <br>
* Switch closed: 1 <br>
* Switch open: 0 <br>
* Failure: -1
* @return success: swi
*
* @~german
* @brief Setze Position.
*
* Diese Funktion überprüft den Zustand des angeschlossenen Schalters und gibt den Wert zurück. <br>
* Schalter geschlossen: 1 <br>
* Schalter offen: 0 <br>
* Fehler: -1
* @return swi
*/
int gnublin_module_step::getSwitch() {
  unsigned char RxBuf[8];
  int swi = 0;

  if(getFullStatus1() == -1) {
    return -1;
  }


  if(i2c.receive(RxBuf, 8)) {

    if(RxBuf[5] & 0x10) {
      swi = 1;
    } else {
      swi = 0;
    }
    return swi;
  } else {
    return -1;
  }
}

//-------------------getActualPosition----------------
/** @~english
* @brief Get actual position.
*
* This funktion sends the getActualPosition Command to the Chip and returns its actiual position.
* @return actualPosition -1 if failure
*
* @~german
* @brief Aktuelle Position ausgeben.
*
* Diese Funktion sendet den getActualPosition Befehl an den Chip und gibt dessen aktuelle Position zurück.
* @return actualPosition -1 bei Fehler
*/
int gnublin_module_step::getActualPosition() {
  unsigned char RxBuf[8];
  int actualPosition = -1;

  if(getFullStatus2() == -1) {
    return -1;
  }

  if(i2c.receive(RxBuf, 8)) {
    actualPosition = (RxBuf[1] << 8 | RxBuf[2]);
  }
  return actualPosition;
}


//*******************************************************************
//Class for accessing GNUBLIN Module-LCD 4x20
//*******************************************************************

/** @~english 
* @brief Reset the ErrorFlag.
*
* @~german 
* @brief Setzt das ErrorFlag zurück.
*
*/
gnublin_module_lcd::gnublin_module_lcd() 
{
	version = (char *) "0.3";
	error_flag=false;
}

//-------------getErrorMessage-------------
/** @~english 
* @brief Get the last Error Message.
*
* This Funktion returns the last Error Message, which occurred in that Class.
* @return ErrorMessage as c-string
*
* @~german 
* @brief Gibt die letzte Error Nachricht zurück.
*
* Diese Funktion gibt die Letzte Error Nachricht zurück, welche in dieser Klasse gespeichert wurde.
* @return ErrorMessage als c-string
*/
const char *gnublin_module_lcd::getErrorMessage(){
	return ErrorMessage.c_str();
}

//-------------fail-------------
/** @~english 
* @brief Returns the error flag. 
*
* If something went wrong, the flag is true.
* @return bool error_flag
*
* @~german 
* @brief Gibt das Error Flag zurück.
*
* Falls das Error Flag in der Klasse gesetzt wurde, wird true zurück gegeben, anderenfalls false.
* @return bool error_flag
*/
bool gnublin_module_lcd::fail(){
	return error_flag;
}

//-------------setAddress-------------
/** @~english 
* @brief Set the slave address 
*
* With this function you can set the individual I2C Slave-Address of the module.
* @param Address new I2C slave Address
*
* @~german 
* @brief Gibt das Error Flag zurück.
*
* Mit dieser Funktion kann die individuelle I2C Slave-Adresse des Moduls gesetzt werden.
* @param Address neue I2C slave Adresse
*/
void gnublin_module_lcd::setAddress(int Address){
	pca.setAddress(Address);
}

//-------------setDevicefile-------------
/** @~english 
* @brief Set devicefile.
*
* With this function you can change the I2C device file. Default is "/dev/i2c-1"
* @param filename path to the I2C device file
*
* @~german 
* @brief Setzt Device Datei.
*
* Mit dieser Funktion kann die I2C Gerätedatei geändert werden. Standartmäßig wird "/dev/i2c-1" benutzt.
* @param filename Pfad zur I2C Gerätedatei
*/
void gnublin_module_lcd::setDevicefile(std::string filename){
	pca.setDevicefile(filename);
}

//-------------out-------------
/** @~english 
* @brief LCD out.
*
* This Funktion sends the given data and RS/RW pins to the pca9555 chip.
* @param rsrw contains the RS/RW pins
* @param data contains the data to send
* @return success: 1, failure: -1
*
* @~german 
* @brief Sendet Daten
*
* Diese Funktion sendet die übergebenen Daten und die RS/RW pins an den pca9555.
* @param rsrw beinhalten die RS/RW pins
* @param data Daten die an das Display gesendet werden sollen
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_lcd::out(unsigned char rsrw, unsigned char data ){
	if(!pca.writePort(0, data)){			//send data on Port 0
		error_flag=true;
		ErrorMessage = pca.getErrorMessage();
		return -1;
	}
	if(!pca.writePort(1, rsrw)){			//send RS/RW bits on Port 1
		error_flag=true;
		ErrorMessage = pca.getErrorMessage();
		return -1;
	}
	if(!pca.writePort(1, (rsrw | LCD_EN))){	//enable on
		error_flag=true;
		ErrorMessage = pca.getErrorMessage();
		return -1;
	}
	usleep(LCD_ENABLE_US);
	if(!pca.writePort(1, rsrw)){			//enable off
		error_flag=true;
		ErrorMessage = pca.getErrorMessage();
		return -1;
	}
	return 1;
}

//-------------sendData-------------
/** @~english 
* @brief Send Data to the LCD
*
* This Funktion sends the given char to the display.
* @param data contains the data to send
* @return success: 1, failure: -1
*
* @~german 
* @brief Sendet Daten an das Display.
*
* Diese Funktion sendet die übergebenen Daten an das Display.
* @param data Daten die an das Display gesendet werden sollen
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_lcd::sendData(unsigned char data){
        if(!out(LCD_RS, (unsigned char) data)){
			return -1;
		}
        usleep(LCD_WRITEDATA_US);
        return 1;
}

//-------------command-------------
/** @~english 
* @brief Send command to the LCD
*
* This Funktion sends an command to the display.
* @param data the command to send
* @return success: 1, failure: -1
*
* @~german 
* @brief Sendet einen Befehl an das Display.
*
* Diese Funktion sendet die übergebenen Befehle an das Display.
* @param data Befehl die an das Display gesendet werden sollen
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_lcd::command(unsigned char data){
        if(!out(0x00, (unsigned char) data)){
		    return -1;
		}
        usleep(LCD_COMMAND_US);
        return 1;
}

//-------------clear-------------
/** @~english 
* @brief Clear the LCD.
*
* Everythin on the Display will be erased
* @return success: 1, failure: -1
*
* @~german 
* @brief Bereinigt das Display
*
* Alles was auf dem Display angezeigt wird, wird gelöscht.
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_lcd::clear(){
        if(!command(LCD_CLEAR_DISPLAY)){
			return -1;
		}
        usleep(LCD_CLEAR_DISPLAY_MS);
        return 1;
}

//-------------home-------------
/** @~english 
* @brief Curse home command.
*
* Returns the Cursor to the first possition.
* @return success: 1, failure: -1
*
* @~german 
* @brief Setzt den Cursor auf Anfangsposition oben links  
*
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_lcd::home(){
        if(!command(LCD_CURSOR_HOME)){
			return -1;
		}
        usleep(LCD_CURSOR_HOME_MS);
        return 1;
}

//-------------setdisplay-------------
/** @~english 
* @brief Set display command.
*
* @return success: 1, failure: -1
*
* @~german 
* @brief Display Einstellungen Befehl.
*
* Mit dieser Funktion können Cursor-Einstellungen am Display vorgenommen werden. Wenn die Übergabe cursor == 1 ist, wird der Cursor am Display an der momentanen Stelle angezeigt(_). Wird blink == 1 übergeben, blinkt ein viereckiger Cursor. 
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_lcd::setdisplay(int cursor, int blink){
        unsigned char set_display;
        //Display ON/OFF Control
        set_display = LCD_SET_DISPLAY + LCD_DISPLAY_ON;
        if(cursor) set_display = set_display + LCD_CURSOR_ON;
        if(blink) set_display = set_display + LCD_BLINKING_ON;
        if(!command(set_display))
        	return -1;
        return 1;
}

//-------------setcursor-------------
/** @~english 
* @brief Set Cursor Command.
*
* Sets the Cursor to the given Position on the Display.
* @param x The Line 
* @param y The Column 
* @return success: 1, failure: -1
*
* @~german 
* @brief Setzt den Cursor an die übergebene Position auf das Display. 
*
* @param x Die Zeile 
* @param y Die Spalte
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_lcd::setcursor(unsigned char x, unsigned char y){
        unsigned char data;
        switch(x){
                case 1: //1. Zeile
                        data = LCD_SET_DDADR + LCD_DDADR_LINE1 + y;
                        break;

                case 2:    // 2. Zeile
                        data = LCD_SET_DDADR + LCD_DDADR_LINE2 + y;
                        break;

                case 3:    // 3. Zeile
                        data = LCD_SET_DDADR + LCD_DDADR_LINE3 + y;
                        break;

                case 4:    // 4. Zeile
                        data = LCD_SET_DDADR + LCD_DDADR_LINE4 + y;
                         break;

                default:
                		error_flag=true;
						ErrorMessage = "Wrong line/column";
                        return -1;
        }
        if(!command(data)){
        	return -1;
        }
        return 1;
}

//-------------string-------------
/** @~english 
* @brief Sends the string to the display.
*
* @param data string to send 
* @return success: 1, failure: -1
*
* @~german 
* @brief Sendet den String an das Display. 
*
* Dieser Funktion kann ein String übergeben werden, welcher auf dem Display angezeigt werden soll. 
* @param data String zum senden 
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_lcd::string(const char *data){
        while(*data != '\0'){
                if(!sendData( *data++)){
                	return -1;
                }
        }
        return 1;
}

//-------------init-------------
/** @~english 
* @brief Initializes the LCD.
*
* @return success: 1, failure: -1
*
* @~german 
* @brief Initialisiert das LCD.
*
* Wenn diese Funktion aufgerufen wird, wird der pca9555 so eingestellt, um das Display ansteuern zu können. Außerdem werden einige Befehle an das Display gesendet, um es auf das Anzeigen von Text vorbereitet. 
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_lcd::init(){
	//Set Ports as output
	if(!pca.portMode(0, "out")){ 	//Port 0 as Output
		error_flag=true;
		ErrorMessage = pca.getErrorMessage();
		return -1;
	}
	if(!pca.portMode(1, "out")){ 	//Port 1 as Output
		error_flag=true;
		ErrorMessage = pca.getErrorMessage();
		return -1;
	}

	//// initial alle Ausgänge auf Null
	if(!pca.writePort(0, 0x00)){
		error_flag=true;
		ErrorMessage = pca.getErrorMessage();
		return -1;
	}
	if(!pca.writePort(1, 0x00)){
		error_flag=true;
		ErrorMessage = pca.getErrorMessage();
		return -1;
	}
	
	usleep(LCD_BOOTUP_MS);

	//function set
	if(!command(LCD_SET_FUNCTION |
                        LCD_FUNCTION_8BIT |
                        LCD_FUNCTION_2LINE |
                        LCD_FUNCTION_5X7)){
		return -1;
	}
	usleep(LCD_SET_8BITMODE_MS);

	//Display ON/OFF Control
	if(!setdisplay(0, 0)){
		return -1;
	}

	if(!clear()){
		return -1;
	}
	
	//entry mode set
	if(!command(LCD_SET_ENTRY |
			LCD_ENTRY_INCREASE |
			LCD_ENTRY_NOSHIFT)){
		return -1;
	}
	
	return 1;
}
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



/* 

   base64.cpp and base64.h



   Copyright (C) 2004-2008 René Nyffenegger



   This source code is provided 'as-is', without any express or implied

   warranty. In no event will the author be held liable for any damages

   arising from the use of this software.



   Permission is granted to anyone to use this software for any purpose,

   including commercial applications, and to alter it and redistribute it

   freely, subject to the following restrictions:



   1. The origin of this source code must not be misrepresented; you must not

      claim that you wrote the original source code. If you use this source code

      in a product, an acknowledgment in the product documentation would be

      appreciated but is not required.



   2. Altered source versions must be plainly marked as such, and must not be

      misrepresented as being the original source code.



   3. This notice may not be removed or altered from any source distribution.



   René Nyffenegger rene.nyffenegger@adp-gmbh.ch



*/







static const std::string base64_chars = 

             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

             "abcdefghijklmnopqrstuvwxyz"

             "0123456789+/";





static inline bool is_base64(unsigned char c) 

{

  return (isalnum(c) || (c == '+') || (c == '/'));

}



std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) 

{

  std::string ret;

  int i = 0, j = 0;

  unsigned char char_array_3[3], char_array_4[4];



  while (in_len--)

	{

    char_array_3[i++] = *(bytes_to_encode++);

    if (i == 3) 

		{

      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;

      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);

      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

      char_array_4[3] = char_array_3[2] & 0x3f;



      for(i = 0; (i <4) ; i++)

        ret += base64_chars[char_array_4[i]];

      i = 0;

    }

  }



  if (i)

  {

    for(j = i; j < 3; j++)

      char_array_3[j] = '\0';



    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;

    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);

    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

    char_array_4[3] = char_array_3[2] & 0x3f;



    for (j = 0; (j < i + 1); j++)

      ret += base64_chars[char_array_4[j]];



    while((i++ < 3))

      ret += '=';



  }



  return ret;



}



std::string base64_decode(std::string const& encoded_string) 

{

  int in_len = encoded_string.size();

  int i = 0, j = 0, in_ = 0;

  unsigned char char_array_4[4], char_array_3[3];

  std::string ret;



  while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) 

	{

    char_array_4[i++] = encoded_string[in_]; in_++;

    if (i ==4) {

      for (i = 0; i <4; i++)

        char_array_4[i] = base64_chars.find(char_array_4[i]);



      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);

      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);

      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];



      for (i = 0; (i < 3); i++)

        ret += char_array_3[i];

      i = 0;

    }

  }



  if (i) 

	{

    for (j = i; j <4; j++)

      char_array_4[j] = 0;



    for (j = 0; j <4; j++)

      char_array_4[j] = base64_chars.find(char_array_4[j]);



    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);

    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);

    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];



    for (j = 0; (j < i - 1); j++) 

			ret += char_array_3[j];

  }



  return ret;

}

////////////////////////////////////////////////////////////////////////////////
// Original class CFastSmtp written by 
// christopher w. backen <immortal@cox.net>
// More details at: http://www.codeproject.com/KB/IP/zsmtp.aspx
// 
// Modifications introduced by Jakub Piwowarczyk:
// 1. name of the class and functions
// 2. new functions added: SendData,ReceiveData and more
// 3. authentication added
// 4. attachments added
// 5 .comments added
// 6. DELAY_IN_MS removed (no delay during sending the message)
// 7. non-blocking mode
// More details at: http://www.codeproject.com/KB/mcpp/CSmtp.aspx
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//        NAME: CSmtp
// DESCRIPTION: Constructor of CSmtp class.
//   ARGUMENTS: none
// USES GLOBAL: none
// MODIFIES GL: m_iXPriority, m_iSMTPSrvPort, RecvBuf, SendBuf
//     RETURNS: none
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-08
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Prepares the Mail-Class
*
* @~german
* @brief Richtet die Mail-Klasse ein
*/

gnublin_smtp::gnublin_smtp()
{
	m_iXPriority = XPRIORITY_NORMAL;
	m_iSMTPSrvPort = 0;
	
	if((RecvBuf = new char[BUFFER_SIZE]) == NULL)
		throw gnublin_smtp(gnublin_smtp::LACK_OF_MEMORY);
	
	if((SendBuf = new char[BUFFER_SIZE]) == NULL)
		throw gnublin_smtp(gnublin_smtp::LACK_OF_MEMORY);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: CSmtp
// DESCRIPTION: Destructor of CSmtp class.
//   ARGUMENTS: none
// USES GLOBAL: RecvBuf, SendBuf
// MODIFIES GL: RecvBuf, SendBuf
//     RETURNS: none
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-08
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief destroys buffer in the Mail-Class
*
* @~german
* @brief Löscht buffer in der Mail-Klasse
*/
gnublin_smtp::~gnublin_smtp()
{
	if(SendBuf)
	{
		delete[] SendBuf;
		SendBuf = NULL;
	}
	if(RecvBuf)
	{
		delete[] RecvBuf;
		RecvBuf = NULL;
	}

}

////////////////////////////////////////////////////////////////////////////////
//        NAME: AddAttachment
// DESCRIPTION: New attachment is added.
//   ARGUMENTS: const char *Path - name of attachment added
// USES GLOBAL: Attachments
// MODIFIES GL: Attachments
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-07
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Add new attachment
*
* @param Path name of the attachment
*
* @~german
* @brief Fügt einen neuen Dateianhang hinzu
*
* @param Path Dateiname
*/
void gnublin_smtp::AddAttachment(const char *Path)
{
	assert(Path);
	Attachments.insert(Attachments.end(),Path);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: AddRecipient
// DESCRIPTION: New recipient data is added i.e.: email and name. .
//   ARGUMENTS: const char *email - mail of the recipient
//              const char *name - name of the recipient
// USES GLOBAL: Recipients
// MODIFIES GL: Recipients
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-07
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief New recipient data is added i.e.: email and name. .
*
* @param email mail of the recipent
* @param name name of the recipent
*
* @~german
* @brief Fügt einen neuen Empfänger hinzu
*
* @param email E-Mail Adresse des Empfängers
* @param name Name des Empfängers
*/
void gnublin_smtp::AddRecipient(const char *email, const char *name)
{	
	if(!email)
		throw gnublin_smtp(gnublin_smtp::UNDEF_RECIPIENT_MAIL);

	Recipient recipient;
	recipient.Mail.insert(0,email);
	name!=NULL ? recipient.Name.insert(0,name) : recipient.Name.insert(0,"");

	Recipients.insert(Recipients.end(), recipient);   
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: AddCCRecipient
// DESCRIPTION: New cc-recipient data is added i.e.: email and name. .
//   ARGUMENTS: const char *email - mail of the cc-recipient
//              const char *name - name of the ccc-recipient
// USES GLOBAL: CCRecipients
// MODIFIES GL: CCRecipients
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-07
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief New cc-recipient data is added i.e.: email and name. .
*
* @param email mail of the cc-recipent
* @param name name of the cc-recipent
*
* @~german
* @brief Fügt einen neuen CC-Empfänger hinzu
*
* @param email E-Mail Adresse des CC-Empfängers
* @param name Name des CC-Empfängers
*/
void gnublin_smtp::AddCCRecipient(const char *email, const char *name)
{	
	if(!email)
		throw gnublin_smtp(gnublin_smtp::UNDEF_RECIPIENT_MAIL);

	Recipient recipient;
	recipient.Mail.insert(0,email);
	name!=NULL ? recipient.Name.insert(0,name) : recipient.Name.insert(0,"");

	CCRecipients.insert(CCRecipients.end(), recipient);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: AddBCCRecipient
// DESCRIPTION: New bcc-recipient data is added i.e.: email and name. .
//   ARGUMENTS: const char *email - mail of the bcc-recipient
//              const char *name - name of the bccc-recipient
// USES GLOBAL: BCCRecipients
// MODIFIES GL: BCCRecipients
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-07
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief New bcc-recipient data is added i.e.: email and name. .
*
* @param email mail of the bcc-recipent
* @param name name of the bcc-recipent
*
* @~german
* @brief Fügt einen neuen BCC-Empfänger hinzu
*
* @param email E-Mail Adresse des BCC-Empfängers
* @param name Name des BCC-Empfängers
*/
void gnublin_smtp::AddBCCRecipient(const char *email, const char *name)
{	
	if(!email)
		throw gnublin_smtp(gnublin_smtp::UNDEF_RECIPIENT_MAIL);

	Recipient recipient;
	recipient.Mail.insert(0,email);
	name!=NULL ? recipient.Name.insert(0,name) : recipient.Name.insert(0,"");

	BCCRecipients.insert(BCCRecipients.end(), recipient);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: AddMsgLine
// DESCRIPTION: Adds new line in a message.
//   ARGUMENTS: const char *Text - text of the new line
// USES GLOBAL: MsgBody
// MODIFIES GL: MsgBody
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-07
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Adds new line in a message.
*
* @param Text text of the new line
*
* @~german
* @brief Fügt eine neue Zeile in die Nachricht ein
*
* @param Text Text der neuen Zeile
*/
void gnublin_smtp::AddMsgLine(const char* Text)
{
	MsgBody.insert(MsgBody.end(),Text);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: DelMsgLine
// DESCRIPTION: Deletes specified line in text message.. .
//   ARGUMENTS: unsigned int Line - line to be delete
// USES GLOBAL: MsgBody
// MODIFIES GL: MsgBody
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-07
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Deletes specified line in text message..
*
* @param Line line to be delete
*
* @~german
* @brief Löscht eine bestimmte Zeile in der Nachricht
*
* @param Line Zeilennummer die gelöscht werden soll
*/
void gnublin_smtp::DelMsgLine(unsigned int Line)
{
	if(Line > MsgBody.size())
		throw gnublin_smtp(gnublin_smtp::OUT_OF_MSG_RANGE);
	MsgBody.erase(MsgBody.begin()+Line);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: DelRecipients
// DESCRIPTION: Deletes all recipients. .
//   ARGUMENTS: void
// USES GLOBAL: Recipients
// MODIFIES GL: Recipients
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-07
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Deletes all recipients.
*
*
* @~german
* @brief Löscht alle Empfänger
*
*/
void gnublin_smtp::DelRecipients()
{
	Recipients.clear();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: DelBCCRecipients
// DESCRIPTION: Deletes all BCC recipients. .
//   ARGUMENTS: void
// USES GLOBAL: BCCRecipients
// MODIFIES GL: BCCRecipients
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-07
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Deletes all BCC-recipients.
*
*
* @~german
* @brief Löscht alle BCC-Empfänger
*
*/
void gnublin_smtp::DelBCCRecipients()
{
	BCCRecipients.clear();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: DelCCRecipients
// DESCRIPTION: Deletes all CC recipients. .
//   ARGUMENTS: void
// USES GLOBAL: CCRecipients
// MODIFIES GL: CCRecipients
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-07
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Deletes all CC-recipients.
*
*
* @~german
* @brief Löscht alle CC-Empfänger
*
*/
void gnublin_smtp::DelCCRecipients()
{
	CCRecipients.clear();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: DelMsgLines
// DESCRIPTION: Deletes message text.
//   ARGUMENTS: void
// USES GLOBAL: MsgBody
// MODIFIES GL: MsgBody
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-07-07
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Deletes message text.
*
*
* @~german
* @brief Löscht den Nachrichtentext
*
*/
void gnublin_smtp::DelMsgLines()
{
	MsgBody.clear();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: DelAttachments
// DESCRIPTION: Deletes all attachments. .
//   ARGUMENTS: void
// USES GLOBAL: Attchments
// MODIFIES GL: Attachments
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-07
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Deletes all attachments.
*
*
* @~german
* @brief Löscht alle Dateianhänge.
*
*/
void gnublin_smtp::DelAttachments()
{
	Attachments.clear();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: AddBCCRecipient
// DESCRIPTION: New bcc-recipient data is added i.e.: email and name. .
//   ARGUMENTS: const char *email - mail of the bcc-recipient
//              const char *name - name of the bccc-recipient
// USES GLOBAL: BCCRecipients
// MODIFIES GL: BCCRecipients, m_oError
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-07
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Edit a specific message line
*
* @param Line Line to edite
* @param Text New text of the specific line
*
* @~german
* @brief Bearbeiten einer bestimmten Nachrichtenzeile
*
* @param Line Zu editierende Zeile
* @param Text Neuer Text der Zeile
*/
void gnublin_smtp::ModMsgLine(unsigned int Line,const char* Text)
{
	if(Text)
	{
		if(Line > MsgBody.size())
			throw gnublin_smtp(gnublin_smtp::OUT_OF_MSG_RANGE);
		MsgBody.at(Line) = std::string(Text);
	}
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: Send
// DESCRIPTION: Sending the mail. .
//   ARGUMENTS: none
// USES GLOBAL: m_sSMTPSrvName, m_iSMTPSrvPort, SendBuf, RecvBuf, m_sLogin,
//              m_sPassword, m_sMailFrom, Recipients, CCRecipients,
//              BCCRecipients, m_sMsgBody, Attachments, 
// MODIFIES GL: SendBuf 
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-08
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Sending the mail.
*
*
* @~german
* @brief E-Mail verschicken.
*
*/
void gnublin_smtp::Send()
{
	unsigned int i,rcpt_count,res,FileId;
	char *FileBuf = NULL, *FileName = NULL;
	FILE* hFile = NULL;
	unsigned long int FileSize,TotalSize,MsgPart;
	bool bAccepted;

	// ***** CONNECTING TO SMTP SERVER *****

	// connecting to remote host:
	if( (hSocket = ConnectRemoteServer(m_sSMTPSrvName.c_str(), m_iSMTPSrvPort)) == INVALID_SOCKET ) 
		throw gnublin_smtp(gnublin_smtp::WSA_INVALID_SOCKET);

	bAccepted = false;
	do
	{
		ReceiveData();
		switch(SmtpXYZdigits())
		{
			case 220:
				bAccepted = true;
				break;
			default:
				throw gnublin_smtp(gnublin_smtp::SERVER_NOT_READY);
		}
	}while(!bAccepted);

	// EHLO <SP> <domain> <CRLF>
	sprintf(SendBuf,"EHLO [127.0.0.1]\r\n");//,GetLocalHostName()!=NULL ? m_sLocalHostName.c_str() : "domain");
	SendData();
	bAccepted = false;
	do
	{
		ReceiveData();
		switch(SmtpXYZdigits())
		{
			case 250:
				bAccepted = true;
				break;
			default:
				throw gnublin_smtp(gnublin_smtp ::COMMAND_EHLO);
		}
	}while(!bAccepted);

	// AUTH <SP> LOGIN <CRLF>
	strcpy(SendBuf,"AUTH LOGIN\r\n");
	SendData();
	bAccepted = false;
	do
	{
		ReceiveData();
		switch(SmtpXYZdigits())
		{
			case 250:
				break;
			case 334:
				bAccepted = true;
				break;
			default:
				throw gnublin_smtp(gnublin_smtp ::COMMAND_AUTH_LOGIN);
		}
	}while(!bAccepted);

	// send login:
	if(!m_sLogin.size())
		throw gnublin_smtp(gnublin_smtp ::UNDEF_LOGIN);
	std::string encoded_login = base64_encode(reinterpret_cast<const unsigned char*>(m_sLogin.c_str()),m_sLogin.size());
	sprintf(SendBuf,"%s\r\n",encoded_login.c_str());
	SendData();
	bAccepted = false;
	do
	{
		ReceiveData();
		switch(SmtpXYZdigits())
		{
			case 334:
				bAccepted = true;
				break;
			default:
				throw gnublin_smtp(gnublin_smtp ::UNDEF_XYZ_RESPONSE);
		}
	}while(!bAccepted);
	
	// send password:
	if(!m_sPassword.size())
		throw gnublin_smtp(gnublin_smtp ::UNDEF_PASSWORD);
	std::string encoded_password = base64_encode(reinterpret_cast<const unsigned char*>(m_sPassword.c_str()),m_sPassword.size());
	sprintf(SendBuf,"%s\r\n",encoded_password.c_str());
	SendData();
	bAccepted = false;
	do
	{
		ReceiveData();
		switch(SmtpXYZdigits())
		{
			case 235:
				bAccepted = true;
				break;
			case 334:
				break;
			case 535:
				throw gnublin_smtp(gnublin_smtp ::BAD_LOGIN_PASS);
			default:
				throw gnublin_smtp(gnublin_smtp ::UNDEF_XYZ_RESPONSE);
		}
	}while(!bAccepted);

	// ***** SENDING E-MAIL *****
	
	// MAIL <SP> FROM:<reverse-path> <CRLF>
	if(!m_sMailFrom.size())
		throw gnublin_smtp(gnublin_smtp ::UNDEF_MAIL_FROM);
	sprintf(SendBuf,"MAIL FROM:<%s>\r\n",m_sMailFrom.c_str());
	SendData();
	bAccepted = false;
	do
	{
		ReceiveData();
		switch(SmtpXYZdigits())
		{
			case 250:
				bAccepted = true;
				break;
			default:
				throw gnublin_smtp(gnublin_smtp ::COMMAND_MAIL_FROM);
		}
	}while(!bAccepted);

	// RCPT <SP> TO:<forward-path> <CRLF>
	if(!(rcpt_count = Recipients.size()))
		throw gnublin_smtp(gnublin_smtp ::UNDEF_RECIPIENTS);
	for(i=0;i<Recipients.size();i++)
	{
		sprintf(SendBuf,"RCPT TO:<%s>\r\n",(Recipients.at(i).Mail).c_str());
		SendData();
		bAccepted = false;
		do
		{
			ReceiveData();
			switch(SmtpXYZdigits())
			{
				case 250:
					bAccepted = true;
					break;
				default:
					rcpt_count--;
			}
		}while(!bAccepted);
	}
	if(rcpt_count <= 0)
		throw gnublin_smtp(gnublin_smtp ::COMMAND_RCPT_TO);

	for(i=0;i<CCRecipients.size();i++)
	{
		sprintf(SendBuf,"RCPT TO:<%s>\r\n",(CCRecipients.at(i).Mail).c_str());
		SendData();
		bAccepted = false;
		do
		{
			ReceiveData();
			switch(SmtpXYZdigits())
			{
				case 250:
					bAccepted = true;
					break;
				default:
					; // not necessary to throw
			}
		}while(!bAccepted);
	}

	for(i=0;i<BCCRecipients.size();i++)
	{
		sprintf(SendBuf,"RCPT TO:<%s>\r\n",(BCCRecipients.at(i).Mail).c_str());
		SendData();
		bAccepted = false;
		do
		{
			ReceiveData();
			switch(SmtpXYZdigits())
			{
				case 250:
					bAccepted = true;
					break;
				default:
					; // not necessary to throw
			}
		}while(!bAccepted);
	}
	
	// DATA <CRLF>
	strcpy(SendBuf,"DATA\r\n");
	SendData();
	bAccepted = false;
	do
	{
		ReceiveData();
		switch(SmtpXYZdigits())
		{
			case 354:
				bAccepted = true;
				break;
			case 250:
				break;
			default:
				throw gnublin_smtp(gnublin_smtp ::COMMAND_DATA);
		}
	}while(!bAccepted);
	
	// send header(s)
	FormatHeader(SendBuf);
	SendData();

	// send text message
	if(GetMsgLines())
	{
		for(i=0;i<GetMsgLines();i++)
		{
			sprintf(SendBuf,"%s\r\n",GetMsgLineText(i));
			SendData();
		}
	}
	else
	{
		sprintf(SendBuf,"%s\r\n"," ");
		SendData();
	}

	// next goes attachments (if they are)
	if((FileBuf = new char[55]) == NULL)
		throw gnublin_smtp(gnublin_smtp ::LACK_OF_MEMORY);

	if((FileName = new char[255]) == NULL)
		throw gnublin_smtp(gnublin_smtp ::LACK_OF_MEMORY);

	TotalSize = 0;
	for(FileId=0;FileId<Attachments.size();FileId++)
	{
		strcpy(FileName,Attachments[FileId].c_str());

		sprintf(SendBuf,"--%s\r\n",BOUNDARY_TEXT);
		strcat(SendBuf,"Content-Type: application/x-msdownload; name=\"");
		strcat(SendBuf,&FileName[Attachments[FileId].find_last_of("\\") + 1]);
		strcat(SendBuf,"\"\r\n");
		strcat(SendBuf,"Content-Transfer-Encoding: base64\r\n");
		strcat(SendBuf,"Content-Disposition: attachment; filename=\"");
		strcat(SendBuf,&FileName[Attachments[FileId].find_last_of("\\") + 1]);
		strcat(SendBuf,"\"\r\n");
		strcat(SendBuf,"\r\n");

		SendData();

		// opening the file:
		hFile = fopen(FileName,"rb");
		if(hFile == NULL)
			throw gnublin_smtp(gnublin_smtp ::FILE_NOT_EXIST);
		
		// checking file size:
		FileSize = 0;
		while(!feof(hFile))
			FileSize += fread(FileBuf,sizeof(char),54,hFile);
		TotalSize += FileSize;

		// sending the file:
		if(TotalSize/1024 > MSG_SIZE_IN_MB*1024)
			throw gnublin_smtp(gnublin_smtp ::MSG_TOO_BIG);
		else
		{
			fseek (hFile,0,SEEK_SET);

			MsgPart = 0;
			for(i=0;i<FileSize/54+1;i++)
			{

				res = fread(FileBuf,sizeof(char),54,hFile);
				MsgPart ? strcat(SendBuf,base64_encode(reinterpret_cast<const unsigned char*>(FileBuf),res).c_str())
					      : strcpy(SendBuf,base64_encode(reinterpret_cast<const unsigned char*>(FileBuf),res).c_str());
				strcat(SendBuf,"\r\n");
				MsgPart += res + 2;
				if(MsgPart >= BUFFER_SIZE/2)
				{ // sending part of the message
					MsgPart = 0;
					SendData(); // FileBuf, FileName, fclose(hFile);
				}
			}
			if(MsgPart)
			{
				SendData(); // FileBuf, FileName, fclose(hFile);
			}
		}
		fclose(hFile);
	}
	delete[] FileBuf;
	delete[] FileName;
	
	// sending last message block (if there is one or more attachments)
	if(Attachments.size())
	{
		sprintf(SendBuf,"\r\n--%s--\r\n",BOUNDARY_TEXT);
		SendData();
	}
	
	// <CRLF> . <CRLF>
	strcpy(SendBuf,"\r\n.\r\n");
	SendData();
	bAccepted = false;
	do
	{
		ReceiveData();
		switch(SmtpXYZdigits())
		{
			case 250:
				bAccepted = true;
				break;
			default:
				throw gnublin_smtp(gnublin_smtp ::MSG_BODY_ERROR);
		}
	}while(!bAccepted);

	// ***** CLOSING CONNECTION *****
	
	// QUIT <CRLF>
	strcpy(SendBuf,"QUIT\r\n");
	SendData();
	bAccepted = false;
	do
	{
		ReceiveData();
		switch(SmtpXYZdigits())
		{
			case 221:
				bAccepted = true;
				break;
			default:
				throw gnublin_smtp(gnublin_smtp ::COMMAND_QUIT);
		}
	}while(!bAccepted);


	close(hSocket);
	//hSocket = NULL;
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: ConnectRemoteServer
// DESCRIPTION: Connecting to the service running on the remote server. 
//   ARGUMENTS: const char *server - service name
//              const unsigned short port - service port
// USES GLOBAL: m_pcSMTPSrvName, m_iSMTPSrvPort, SendBuf, RecvBuf, m_pcLogin,
//              m_pcPassword, m_pcMailFrom, Recipients, CCRecipients,
//              BCCRecipients, m_pcMsgBody, Attachments, 
// MODIFIES GL: m_oError 
//     RETURNS: socket of the remote service
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Connecting to the service running on the remote server. 
*
* @param server server name
* @param port service port
* @return socket of the remote service
*
* @~german
* @brief Verbindung mit dem Service auf dem Remote Server aufnehmen
*
* @param server Servername
* @param Port Port des Service
* @return Socket des Remote Server
*/
SOCKET gnublin_smtp::ConnectRemoteServer(const char *szServer,const unsigned short nPort_)
{
	unsigned short nPort = 0;
	LPSERVENT lpServEnt;
	SOCKADDR_IN sockAddr;
	unsigned long ul = 1;
	fd_set fdwrite,fdexcept;
	timeval timeout;
	int res = 0;

	timeout.tv_sec = TIME_IN_SEC;
	timeout.tv_usec = 0;

	SOCKET hSocket = INVALID_SOCKET;

	if((hSocket = socket(PF_INET, SOCK_STREAM,0)) == INVALID_SOCKET)
		throw gnublin_smtp(gnublin_smtp ::WSA_INVALID_SOCKET);

	if(nPort_ != 0)
		nPort = htons(nPort_);
	else
	{
		lpServEnt = getservbyname("mail", 0);
		if (lpServEnt == NULL)
			nPort = htons(25);
		else 
			nPort = lpServEnt->s_port;
	}
			
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = nPort;
	if((sockAddr.sin_addr.s_addr = inet_addr(szServer)) == INADDR_NONE)
	{
		LPHOSTENT host;
			
		host = gethostbyname(szServer);
		if (host)
			memcpy(&sockAddr.sin_addr,host->h_addr_list[0],host->h_length);
		else
		{
			close(hSocket);
			throw gnublin_smtp(gnublin_smtp ::WSA_GETHOSTBY_NAME_ADDR);
		}				
	}

	// start non-blocking mode for socket:
	if(ioctl(hSocket,FIONBIO, (unsigned long*)&ul) == SOCKET_ERROR)
	{
		close(hSocket);
		throw gnublin_smtp(gnublin_smtp ::WSA_IOCTLSOCKET);
	}

	if(connect(hSocket,(LPSOCKADDR)&sockAddr,sizeof(sockAddr)) == SOCKET_ERROR)
	{
		if(errno != EINPROGRESS)

		{
			close(hSocket);
			throw gnublin_smtp(gnublin_smtp ::WSA_CONNECT);
		}
	}
	else
		return hSocket;

	while(true)
	{
		FD_ZERO(&fdwrite);
		FD_ZERO(&fdexcept);

		FD_SET(hSocket,&fdwrite);
		FD_SET(hSocket,&fdexcept);

		if((res = select(hSocket+1,NULL,&fdwrite,&fdexcept,&timeout)) == SOCKET_ERROR)
		{
			close(hSocket);
			throw gnublin_smtp(gnublin_smtp ::WSA_SELECT);
		}

		if(!res)
		{
			close(hSocket);
			throw gnublin_smtp(gnublin_smtp ::SELECT_TIMEOUT);
		}
		if(res && FD_ISSET(hSocket,&fdwrite))
			break;
		if(res && FD_ISSET(hSocket,&fdexcept))
		{
			close(hSocket);
			throw gnublin_smtp(gnublin_smtp ::WSA_SELECT);
		}
	} // while

	FD_CLR(hSocket,&fdwrite);
	FD_CLR(hSocket,&fdexcept);

	return hSocket;
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: SmtpXYZdigits
// DESCRIPTION: Converts three letters from RecvBuf to the number.
//   ARGUMENTS: none
// USES GLOBAL: RecvBuf
// MODIFIES GL: none
//     RETURNS: integer number
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Converts three letters from RecvBuf to the number.
* @return integer number
*
* @~german
* @brief Konvertiert drei Buchstaben des RecvBuf in Zahlen.
* @return Zahl im integer-Format
*/
int gnublin_smtp::SmtpXYZdigits()
{
	assert(RecvBuf);
	if(RecvBuf == NULL)
		return 0;
	return (RecvBuf[0]-'0')*100 + (RecvBuf[1]-'0')*10 + RecvBuf[2]-'0';
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: FormatHeader
// DESCRIPTION: Prepares a header of the message.
//   ARGUMENTS: char* header - formated header string
// USES GLOBAL: Recipients, CCRecipients, BCCRecipients
// MODIFIES GL: none
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-07
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Prepares a header of the message.
*
* @param header formated header string
*
* @~german
* @brief Bereitet den Header der Nachricht vor.
*
* @param header Formatierter Header-String
*/
void gnublin_smtp::FormatHeader(char* header)
{
	char month[][4] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
	size_t i;
	std::string to;
	std::string cc;
	std::string bcc;
	time_t rawtime;
	struct tm* timeinfo;

	// date/time check
	if(time(&rawtime) > 0)
		timeinfo = localtime(&rawtime);
	else
		throw gnublin_smtp(gnublin_smtp ::TIME_ERROR);

	// check for at least one recipient
	if(Recipients.size())
	{
		for (i=0;i<Recipients.size();i++)
		{
			if(i > 0)
				to.append(",");
			to += Recipients[i].Name;
			to.append("<");
			to += Recipients[i].Mail;
			to.append(">");
		}
	}
	else
		throw gnublin_smtp(gnublin_smtp ::UNDEF_RECIPIENTS);

	if(CCRecipients.size())
	{
		for (i=0;i<CCRecipients.size();i++)
		{
			if(i > 0)
				cc. append(",");
			cc += CCRecipients[i].Name;
			cc.append("<");
			cc += CCRecipients[i].Mail;
			cc.append(">");
		}
	}

	if(BCCRecipients.size())
	{
		for (i=0;i<BCCRecipients.size();i++)
		{
			if(i > 0)
				bcc.append(",");
			bcc += BCCRecipients[i].Name;
			bcc.append("<");
			bcc += BCCRecipients[i].Mail;
			bcc.append(">");
		}
	}
	
	// Date: <SP> <dd> <SP> <mon> <SP> <yy> <SP> <hh> ":" <mm> ":" <ss> <SP> <zone> <CRLF>
	sprintf(header,"Date: %d %s %d %d:%d:%d\r\n",	timeinfo->tm_mday,
																								month[timeinfo->tm_mon],
																								timeinfo->tm_year+1900,
																								timeinfo->tm_hour,
																								timeinfo->tm_min,
																								timeinfo->tm_sec); 
	
	// From: <SP> <sender>  <SP> "<" <sender-email> ">" <CRLF>
	if(!m_sMailFrom.size())
		throw gnublin_smtp(gnublin_smtp ::UNDEF_MAIL_FROM);
	strcat(header,"From: ");
	if(m_sNameFrom.size())
		strcat(header, m_sNameFrom.c_str());
	strcat(header," <");
	if(m_sNameFrom.size())
		strcat(header,m_sMailFrom.c_str());
	else
		strcat(header,"mail@domain.com");
	strcat(header, ">\r\n");

	// X-Mailer: <SP> <xmailer-app> <CRLF>
	if(m_sXMailer.size())
	{
		strcat(header,"X-Mailer: ");
		strcat(header, m_sXMailer.c_str());
		strcat(header, "\r\n");
	}

	// Reply-To: <SP> <reverse-path> <CRLF>
	if(m_sReplyTo.size())
	{
		strcat(header, "Reply-To: ");
		strcat(header, m_sReplyTo.c_str());
		strcat(header, "\r\n");
	}

	// X-Priority: <SP> <number> <CRLF>
	switch(m_iXPriority)
	{
		case XPRIORITY_HIGH:
			strcat(header,"X-Priority: 2 (High)\r\n");
			break;
		case XPRIORITY_NORMAL:
			strcat(header,"X-Priority: 3 (Normal)\r\n");
			break;
		case XPRIORITY_LOW:
			strcat(header,"X-Priority: 4 (Low)\r\n");
			break;
		default:
			strcat(header,"X-Priority: 3 (Normal)\r\n");
	}

	// To: <SP> <remote-user-mail> <CRLF>
	strcat(header,"To: ");
	strcat(header, to.c_str());
	strcat(header, "\r\n");

	// Cc: <SP> <remote-user-mail> <CRLF>
	if(CCRecipients.size())
	{
		strcat(header,"Cc: ");
		strcat(header, cc.c_str());
		strcat(header, "\r\n");
	}

	if(BCCRecipients.size())
	{
		strcat(header,"Bcc: ");
		strcat(header, bcc.c_str());
		strcat(header, "\r\n");
	}

	// Subject: <SP> <subject-text> <CRLF>
	if(!m_sSubject.size()) 
		strcat(header, "Subject:  ");
	else
	{
	  strcat(header, "Subject: ");
	  strcat(header, m_sSubject.c_str());
	}
	strcat(header, "\r\n");
	
	// MIME-Version: <SP> 1.0 <CRLF>
	strcat(header,"MIME-Version: 1.0\r\n");
	if(!Attachments.size())
	{ // no attachments
		strcat(header,"Content-type: text/plain; charset=US-ASCII\r\n");
		strcat(header,"Content-Transfer-Encoding: 7bit\r\n");
		strcat(SendBuf,"\r\n");
	}
	else
	{ // there is one or more attachments
		strcat(header,"Content-Type: multipart/mixed; boundary=\"");
		strcat(header,BOUNDARY_TEXT);
		strcat(header,"\"\r\n");
		strcat(header,"\r\n");
		// first goes text message
		strcat(SendBuf,"--");
		strcat(SendBuf,BOUNDARY_TEXT);
		strcat(SendBuf,"\r\n");
		strcat(SendBuf,"Content-type: text/plain; charset=US-ASCII\r\n");
		strcat(SendBuf,"Content-Transfer-Encoding: 7bit\r\n");
		strcat(SendBuf,"\r\n");
	}

	// done
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: ReceiveData
// DESCRIPTION: Receives a row terminated '\n'.
//   ARGUMENTS: none
// USES GLOBAL: RecvBuf
// MODIFIES GL: RecvBuf
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-07
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Receives a row terminated '\n'.
*
*
* @~german
* @brief Empfängt eine mi '\n' terminierte Zeile.
*
*/
void gnublin_smtp::ReceiveData()
{
	int res,i = 0;
	fd_set fdread;
	timeval time;

	time.tv_sec = TIME_IN_SEC;
	time.tv_usec = 0;

	assert(RecvBuf);

	if(RecvBuf == NULL)
		throw gnublin_smtp(gnublin_smtp ::RECVBUF_IS_EMPTY);

	while(1)
	{
		FD_ZERO(&fdread);

		FD_SET(hSocket,&fdread);

		if((res = select(hSocket+1, &fdread, NULL, NULL, &time)) == SOCKET_ERROR)
		{
			FD_CLR(hSocket,&fdread);
			throw gnublin_smtp(gnublin_smtp ::WSA_SELECT);
		}

		if(!res)
		{
			//timeout
			FD_CLR(hSocket,&fdread);
			throw gnublin_smtp(gnublin_smtp ::SERVER_NOT_RESPONDING);
		}

		if(res && FD_ISSET(hSocket,&fdread))
		{
			if(i >= BUFFER_SIZE)
			{
				FD_CLR(hSocket,&fdread);
				throw gnublin_smtp(gnublin_smtp ::LACK_OF_MEMORY);
			}
			if(recv(hSocket,&RecvBuf[i++],1,0) == SOCKET_ERROR)
			{
				FD_CLR(hSocket,&fdread);
				throw gnublin_smtp(gnublin_smtp ::WSA_RECV);
			}
			if(RecvBuf[i-1]=='\n')
			{
				RecvBuf[i] = '\0';
				break;
			}
		}
	}

	FD_CLR(hSocket,&fdread);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: SendData
// DESCRIPTION: Sends data from SendBuf buffer.
//   ARGUMENTS: none
// USES GLOBAL: SendBuf
// MODIFIES GL: none
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Sends data from SendBuf buffer.
*
*
* @~german
* @brief Sendet die daten des SendBuf.
*
*/
void gnublin_smtp::SendData()
{
	int idx = 0,res,nLeft = strlen(SendBuf);
	fd_set fdwrite;
	timeval time;

	time.tv_sec = TIME_IN_SEC;
	time.tv_usec = 0;

	assert(SendBuf);

	if(SendBuf == NULL)
		throw gnublin_smtp(gnublin_smtp ::SENDBUF_IS_EMPTY);

	while(1)
	{
		FD_ZERO(&fdwrite);

		FD_SET(hSocket,&fdwrite);

		if((res = select(hSocket+1,NULL,&fdwrite,NULL,&time)) == SOCKET_ERROR)
		{
			FD_CLR(hSocket,&fdwrite);
			throw gnublin_smtp(gnublin_smtp ::WSA_SELECT);
		}

		if(!res)
		{
			//timeout
			FD_CLR(hSocket,&fdwrite);
			throw gnublin_smtp(gnublin_smtp ::SERVER_NOT_RESPONDING);
		}

		if(res && FD_ISSET(hSocket,&fdwrite))
		{
			if(nLeft > 0)
			{
				if((res = send(hSocket,&SendBuf[idx],nLeft,0)) == SOCKET_ERROR)
				{
					FD_CLR(hSocket,&fdwrite);
					throw gnublin_smtp(gnublin_smtp ::WSA_SEND);
				}
				if(!res)
					break;
				nLeft -= res;
				idx += res;
			}
			else
				break;
		}
	}

	FD_CLR(hSocket,&fdwrite);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: GetLocalHostName
// DESCRIPTION: Returns local host name. 
//   ARGUMENTS: none
// USES GLOBAL: m_pcLocalHostName
// MODIFIES GL: m_oError, m_pcLocalHostName 
//     RETURNS: socket of the remote service
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Returns local host name. 
* @return socket of the remote service
*
* @~german
* @brief Gibt den lokalen Host Namen zurück.
* @return Socket des Remote Service
*/
const char* gnublin_smtp::GetLocalHostName() const
{
	char* str = NULL;

	if((str = new char[255]) == NULL)
		throw gnublin_smtp(gnublin_smtp ::LACK_OF_MEMORY);
	if(gethostname(str,255) == SOCKET_ERROR)
	{
		delete[] str;
		throw gnublin_smtp(gnublin_smtp ::WSA_HOSTNAME);
	}
	delete[] str;
	return m_sLocalHostName.c_str();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: GetRecipientCount
// DESCRIPTION: Returns the number of recipents.
//   ARGUMENTS: none
// USES GLOBAL: Recipients
// MODIFIES GL: none 
//     RETURNS: number of recipents
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Returns the number of recipents.
* @return number of recipents.
*
* @~german
* @brief Gibt die anzahl der Empfänger zurück.
* @return Anzahl der Empfänger
*/
unsigned int gnublin_smtp::GetRecipientCount() const
{
	return Recipients.size();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: GetBCCRecipientCount
// DESCRIPTION: Returns the number of bcc-recipents. 
//   ARGUMENTS: none
// USES GLOBAL: BCCRecipients
// MODIFIES GL: none 
//     RETURNS: number of bcc-recipents
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Returns the number of BCC-recipents.
* @return number of BCC-recipents.
*
* @~german
* @brief Gibt die anzahl der BCC-Empfänger zurück.
* @return Anzahl der BCC-Empfänger
*/
unsigned int gnublin_smtp::GetBCCRecipientCount() const
{
	return BCCRecipients.size();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: GetCCRecipientCount
// DESCRIPTION: Returns the number of cc-recipents.
//   ARGUMENTS: none
// USES GLOBAL: CCRecipients
// MODIFIES GL: none 
//     RETURNS: number of cc-recipents
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Returns the number of CC-recipents.
* @return number of CC-recipents.
*
* @~german
* @brief Gibt die anzahl der CC-Empfänger zurück.
* @return Anzahl der CC-Empfänger
*/
unsigned int gnublin_smtp::GetCCRecipientCount() const
{
	return CCRecipients.size();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: GetReplyTo
// DESCRIPTION: Returns m_pcReplyTo string.
//   ARGUMENTS: none
// USES GLOBAL: m_sReplyTo
// MODIFIES GL: none 
//     RETURNS: m_sReplyTo string
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Returns m_pcReplyTo string.
* @return m_pcReplyTo string
*
* @~german
* @brief Gibt den m_pcReplyTo string zurück.
* @return m_pcReplyTo string
*/
const char* gnublin_smtp::GetReplyTo() const
{
	return m_sReplyTo.c_str();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: GetMailFrom
// DESCRIPTION: Returns m_pcMailFrom string.
//   ARGUMENTS: none
// USES GLOBAL: m_sMailFrom
// MODIFIES GL: none 
//     RETURNS: m_sMailFrom string
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Returns m_pcMailFrom string.
* @return m_pcMailFrom string
*
* @~german
* @brief Gibt den m_pcMailFrom string zurück.
* @return m_pcMailFrom string
*/
const char* gnublin_smtp::GetMailFrom() const
{
	return m_sMailFrom.c_str();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: GetSenderName
// DESCRIPTION: Returns m_pcNameFrom string.
//   ARGUMENTS: none
// USES GLOBAL: m_sNameFrom
// MODIFIES GL: none 
//     RETURNS: m_sNameFrom string
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Returns m_pcNameFrom string.
* @return m_pcNameFrom string
*
* @~german
* @brief Gibt den m_pcNameFrom string zurück.
* @return m_pcNameFrom string
*/
const char* gnublin_smtp::GetSenderName() const
{
	return m_sNameFrom.c_str();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: GetSubject
// DESCRIPTION: Returns m_pcSubject string.
//   ARGUMENTS: none
// USES GLOBAL: m_sSubject
// MODIFIES GL: none 
//     RETURNS: m_sSubject string
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Returns m_pcSubject string.
* @return m_pcSubject string
*
* @~german
* @brief Gibt den m_pcSubject string zurück.
* @return m_pcSubject string
*/
const char* gnublin_smtp::GetSubject() const
{
	return m_sSubject.c_str();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: GetXMailer
// DESCRIPTION: Returns m_pcXMailer string.
//   ARGUMENTS: none
// USES GLOBAL: m_pcXMailer
// MODIFIES GL: none 
//     RETURNS: m_pcXMailer string
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Returns m_pcXMailer string.
* @return m_pcXMailer string
*
* @~german
* @brief Gibt den m_pcXMailer string zurück.
* @return m_pcXMailer string
*/
const char* gnublin_smtp::GetXMailer() const
{
	return m_sXMailer.c_str();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: GetXPriority
// DESCRIPTION: Returns m_iXPriority string.
//   ARGUMENTS: none
// USES GLOBAL: m_iXPriority
// MODIFIES GL: none 
//     RETURNS: CSmptXPriority m_pcXMailer
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Returns m_pcXPriority string.
* @return m_pcXPriority string
*
* @~german
* @brief Gibt den m_pcXPriority string zurück.
* @return m_pcXPriority string
*/
CSmptXPriority gnublin_smtp::GetXPriority() const
{
	return m_iXPriority;
}

/**
* @~english
* @brief Returns text of a specific message Line
* @param Line Linenumber
* @return Text
*
* @~german
* @brief Gibt Text einer bestimmten Nachrichtenzeile zurück
* @param Line Zeilennummer
* @return Text
*/
const char* gnublin_smtp::GetMsgLineText(unsigned int Line) const
{
	if(Line > MsgBody.size())
		throw gnublin_smtp(gnublin_smtp ::OUT_OF_MSG_RANGE);
	return MsgBody.at(Line).c_str();
}

unsigned int gnublin_smtp::GetMsgLines() const
{
	return MsgBody.size();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: SetXPriority
// DESCRIPTION: Setting priority of the message.
//   ARGUMENTS: CSmptXPriority priority - priority of the message (	XPRIORITY_HIGH,
//              XPRIORITY_NORMAL, XPRIORITY_LOW)
// USES GLOBAL: none
// MODIFIES GL: m_iXPriority 
//     RETURNS: none
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Setting priority of the message.
* @param priority Priority of the message (XPRIORITY_HIGH, XPRIORITY_NORMAL, XPRIORITY_LOW)
*
* @~german
* @brief Priorität der Nachricht festlegen
* @param priority Priorität der Nachricht (XPRIORITY_HIGH, XPRIORITY_NORMAL, XPRIORITY_LOW)
*/
void gnublin_smtp::SetXPriority(CSmptXPriority priority)
{
	m_iXPriority = priority;
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: SetReplyTo
// DESCRIPTION: Setting the return address.
//   ARGUMENTS: const char *ReplyTo - return address
// USES GLOBAL: m_sReplyTo
// MODIFIES GL: m_sReplyTo
//     RETURNS: none
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-08
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Setting the return address.
* @param ReplyTo return address
*
* @~german
* @brief Rücksendeadresse festlegen
* @param ReplyTo Rücksendeadresse
*/
void gnublin_smtp::SetReplyTo(const char *ReplyTo)
{
	//m_sReplyTo.erase();
	m_sReplyTo.insert(0,ReplyTo);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: SetSenderMail
// DESCRIPTION: Setting sender's mail.
//   ARGUMENTS: const char *EMail - sender's e-mail
// USES GLOBAL: m_sMailFrom
// MODIFIES GL: m_sMailFrom
//     RETURNS: none
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-08
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Setting sender's mail.
* @param E-Mail sender's e-mail
*
* @~german
* @brief Absenderadresse setzen
* @param E-Mail Absenderadresse
*/
void gnublin_smtp::SetSenderMail(const char *EMail)
{
	m_sMailFrom.erase();
	m_sMailFrom.insert(0,EMail);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: SetSenderName
// DESCRIPTION: Setting sender's name.
//   ARGUMENTS: const char *Name - sender's name
// USES GLOBAL: m_sNameFrom
// MODIFIES GL: m_sNameFrom
//     RETURNS: none
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-08
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Setting sender's name.
* @param Name sender's name
*
* @~german
* @brief Absendername setzen
* @param Name Absendername
*/
void gnublin_smtp::SetSenderName(const char *Name)
{
	m_sNameFrom.erase();
	m_sNameFrom.insert(0,Name);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: SetSubject
// DESCRIPTION: Setting subject of the message.
//   ARGUMENTS: const char *Subject - subject of the message
// USES GLOBAL: m_sSubject
// MODIFIES GL: m_sSubject
//     RETURNS: none
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-08
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Setting subject of message.
* @param Subject subject of the message
*
* @~german
* @brief Betraff der Nachricht festlegen
* @param Subject Betreff
*/
void gnublin_smtp::SetSubject(const char *Subject)
{
	m_sSubject.erase();
	m_sSubject.insert(0,Subject);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: SetXMailer
// DESCRIPTION: Setting the name of program which is sending the mail.
//   ARGUMENTS: const char *XMailer - programe name
// USES GLOBAL: m_sXMailer
// MODIFIES GL: m_sXMailer
//     RETURNS: none
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-08
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Setting the name of program which is sending the mail.
* @param XMailer programe name
*
* @~german
* @brief Name des Absendeprogramms bestimmen
* @param XMailer Programmname
*/
void gnublin_smtp::SetXMailer(const char *XMailer)
{
	m_sXMailer.erase();
	m_sXMailer.insert(0,XMailer);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: SetLogin
// DESCRIPTION: Setting the login of SMTP account's owner.
//   ARGUMENTS: const char *Login - login of SMTP account's owner
// USES GLOBAL: m_sLogin
// MODIFIES GL: m_sLogin
//     RETURNS: none
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-08
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Setting the login of SMTP account's owner.
* @param Login login of SMTP account's owner
*
* @~german
* @brief SMTP Benutzername angeben
* @param Login Benutzername
*/
void gnublin_smtp::SetLogin(const char *Login)
{
	m_sLogin.erase();
	m_sLogin.insert(0,Login);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: SetPassword
// DESCRIPTION: Setting the password of SMTP account's owner.
//   ARGUMENTS: const char *Password - password of SMTP account's owner
// USES GLOBAL: m_sPassword
// MODIFIES GL: m_sPassword
//     RETURNS: none
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-08
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Setting the password of SMTP account's owner.
* @param Password password of SMTP account's owner
*
* @~german
* @brief Passwort des SMTP accounts angeben
* @param Password Passwort des Accounts
*/
void gnublin_smtp::SetPassword(const char *Password)
{
	m_sPassword.erase();
	m_sPassword.insert(0,Password);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: SetSMTPServer
// DESCRIPTION: Setting the SMTP service name and port.
//   ARGUMENTS: const char* SrvName - SMTP service name
//              const unsigned short SrvPort - SMTO service port
// USES GLOBAL: m_sSMTPSrvName
// MODIFIES GL: m_sSMTPSrvName 
//     RETURNS: none
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JO 2010-0708
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Setting the SMTP service name and port.
* @param SrvName SMTP service name
* @param SrvPort SMTP service port
*
* @~german
* @brief SMTP Server setzen
* @param SrvName Servername
* @param SrvPort Serverport
*/
void gnublin_smtp::SetSMTPServer(const char* SrvName,const unsigned short SrvPort)
{
	m_iSMTPSrvPort = SrvPort;
	m_sSMTPSrvName.erase();
	m_sSMTPSrvName.insert(0,SrvName);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: GetErrorText (friend function)
// DESCRIPTION: Returns the string for specified error code.
//   ARGUMENTS: CSmtpError ErrorId - error code
// USES GLOBAL: none
// MODIFIES GL: none 
//     RETURNS: error string
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Returns the string for specified error code.
* @return error string
*
* @~german
* @brief Gibt einen String mit einem spezifischem Fehlercode zurück.
* @return Fehlerstring
*/
std::string gnublin_smtp::GetErrorText() const
{
	switch(ErrorCode)
	{
		case gnublin_smtp::CSMTP_NO_ERROR:
			return "";
		case gnublin_smtp::WSA_STARTUP:
			return "Unable to initialise winsock2";
		case gnublin_smtp::WSA_VER:
			return "Wrong version of the winsock2";
		case gnublin_smtp::WSA_SEND:
			return "Function send() failed";
		case gnublin_smtp::WSA_RECV:
			return "Function recv() failed";
		case gnublin_smtp::WSA_CONNECT:
			return "Function connect failed";
		case gnublin_smtp::WSA_GETHOSTBY_NAME_ADDR:
			return "Unable to determine remote server";
		case gnublin_smtp::WSA_INVALID_SOCKET:
			return "Invalid winsock2 socket";
		case gnublin_smtp::WSA_HOSTNAME:
			return "Function hostname() failed";
		case gnublin_smtp::WSA_IOCTLSOCKET:
			return "Function ioctlsocket() failed";
		case gnublin_smtp::BAD_IPV4_ADDR:
			return "Improper IPv4 address";
		case gnublin_smtp::UNDEF_MSG_HEADER:
			return "Undefined message header";
		case gnublin_smtp::UNDEF_MAIL_FROM:
			return "Undefined mail sender";
		case gnublin_smtp::UNDEF_SUBJECT:
			return "Undefined message subject";
		case gnublin_smtp::UNDEF_RECIPIENTS:
			return "Undefined at least one reciepent";
		case gnublin_smtp::UNDEF_RECIPIENT_MAIL:
			return "Undefined recipent mail";
		case gnublin_smtp::UNDEF_LOGIN:
			return "Undefined user login";
		case gnublin_smtp::UNDEF_PASSWORD:
			return "Undefined user password";
		case gnublin_smtp::COMMAND_MAIL_FROM:
			return "Server returned error after sending MAIL FROM";
		case gnublin_smtp::COMMAND_EHLO:
			return "Server returned error after sending EHLO";
		case gnublin_smtp::COMMAND_AUTH_LOGIN:
			return "Server returned error after sending AUTH LOGIN";
		case gnublin_smtp::COMMAND_DATA:
			return "Server returned error after sending DATA";
		case gnublin_smtp::COMMAND_QUIT:
			return "Server returned error after sending QUIT";
		case gnublin_smtp::COMMAND_RCPT_TO:
			return "Server returned error after sending RCPT TO";
		case gnublin_smtp::MSG_BODY_ERROR:
			return "Error in message body";
		case gnublin_smtp::CONNECTION_CLOSED:
			return "Server has closed the connection";
		case gnublin_smtp::SERVER_NOT_READY:
			return "Server is not ready";
		case gnublin_smtp::SERVER_NOT_RESPONDING:
			return "Server not responding";
		case gnublin_smtp::FILE_NOT_EXIST:
			return "File not exist";
		case gnublin_smtp::MSG_TOO_BIG:
			return "Message is too big";
		case gnublin_smtp::BAD_LOGIN_PASS:
			return "Bad login or password";
		case gnublin_smtp::UNDEF_XYZ_RESPONSE:
			return "Undefined xyz SMTP response";
		case gnublin_smtp::LACK_OF_MEMORY:
			return "Lack of memory";
		case gnublin_smtp::TIME_ERROR:
			return "time() error";
		case gnublin_smtp::RECVBUF_IS_EMPTY:
			return "RecvBuf is empty";
		case gnublin_smtp::SENDBUF_IS_EMPTY:
			return "SendBuf is empty";
		case gnublin_smtp::OUT_OF_MSG_RANGE:
			return "Specified line number is out of message size";
		default:
			return "Undefined error id";
	}
}


//***************************************************************************
// Class for creating csv files
//***************************************************************************

/**
* @~english
* @brief Default delimiters are set if necessary.
*
* Default delimiters:
* delimiterRowSign: \r\n
* delimiterColumnSign: ;
* delimiterFieldSign: "
* @~german
* @brief Standard Trennsymbole werden falls nötig gesetzt
*
* Standard Trennsymbole:
* delimiterRowSign: \r\n
* delimiterColumnSign: ;
* delimiterFieldSign: "
*
*/


//******************** constructor ******************************************
/**
* @~english
* @brief Set Default delimiters
*
* @~german
* @brief Setzt Default Trennsymbole
*
*/

gnublin_csv::gnublin_csv() {
	user_file_handle = true;
	delimiterRowSign = "\r\n";
	delimiterColumnSign = ';';
	delimiterFieldSign = '"';
}


//******************** overloaded constructor *******************************
/**
 * @~english
 * @brief Set Default delimiters and creates a csv file. addRow() will automatically open() and close() the file at each call.
 * 
 * @param new_filename name of the file
 *
 * @~german
 * @brief Setzt Default Trennsymbole und erstellt eine csv Datei. open() und close() wird automatisch bei jedem Aufruf von addRow() ausgeführt.
 *
 * @param new_filename Name der Datei
 *
 */

gnublin_csv::gnublin_csv(string new_filename) {
	user_file_handle = false;
	filename = new_filename;

	ofstream file(filename.c_str());
  if (file < 0) {
  }
  file.close();

  delimiterRowSign = "\r\n";
  delimiterColumnSign = ';';
  delimiterFieldSign = '"';
}


//********************** open ***********************************************
/**
 *
 * @~english
 * @brief Creates a csv file. Use this method in combination with default constructer and close() for faster access. (No automated open() and close() in addRow() )
 *  
 * @param new_filename name of the file
 * @return 1 by success, -1 by failure
 *
 * @~german
 * @brief Erstellt eine csv Datei. Benutze diese Methode in Kombination mit dem Default Konstruktor und close() um schnelleren Dateizugriff zu ermöglichen. (Kein automatisches open() und close() in addRow() )
 *
 * @param new_filename Name der Datei
 * @return 1 bei Erfolg, -1 im Fehlerfall
 *
 */

int gnublin_csv::open(string new_filename) {
	filename = new_filename;
	
  std::ofstream file (filename.c_str());
  if (file < 0) {
    return -1;
  }
	return 0;
}


//********************** addRow *********************************************
/**
 *
 * @~english
 * @brief Adds a row to the csv file. Example: gnublin_csv csv; string str = "Hello"; float f = 3.2; addRow(2, str.c_str(), (csv.NumberToString(f)).c_str());
 *  
 * @param quantity number of strings
 * @param ... C-strings, which should be written to the file. (Convert C++-Strings with str.c_str(); and numbers with NumberToString() memeber to Strings)
 * @return 1 by success, -1 by failure
 *
 * @~german
 * @brief Fügt eine Zeile in der csv Datei hinzu. Beispiel: gnublin_csv csv; string str = "Hello"; float f = 3.2; addRow(2, str.c_str(), (csv.NumberToString(f)).c_str());
 *
 * @param quantity Anzahl der Strings
 * @param ... C-Strings, welche in die Datei geschrieben werden sollen. (Konvertiere C++-Strings mit str.c_str(); und Zahlen mit NumberToString zu Strings) 
 * @return 1 bei Erfolg, -1 im Fehlerfall
 *
 */

int gnublin_csv::addRow(int quantity, ...) {
	ofstream file (filename.c_str(), ios::out|ios::app);
	va_list params;
	char* par;
	va_start(params, quantity);
	for (int i=0; i<quantity; i++) {
		par = va_arg(params, char*);
		if (delimiterFieldSign != '\0') {
			file << delimiterFieldSign << par << delimiterFieldSign << delimiterColumnSign;
		} else {
			file << par << delimiterColumnSign;
		}
	}	

	file << delimiterRowSign;
	va_end(params);
	if (!user_file_handle) {
		file.close();
	}
	return 0;	
}

//********************** close **********************************************
/**
 *
 * @~english
 * @brief Closes file. Use this method in combination with default constructor and open() for faster access. (No automated open and close in addRow() )
 *
 * @return 1 by success, -1 by failure
 * 
 * @~german
 * @brief Schließt die Datei. Benutze diese Methode in Kombination mit dem Default Konstruktur und open() um schnelleren Dateizugriff zu ermöglichen. (Kein automatisches open() und close() in addRow() )
 *
 * @return 1 bei Erfolg, -1 im Fehlerfall
 *
 */

void gnublin_csv::close() {
	ofstream file (filename.c_str(), ios::out|ios::app);
	file.close();
}

//********************** delimiterRow ***************************************
/**
 *
 * @~english
 * @brief Specifies the delimiter of each row
 *  
 * @param delimiterSign Sign which should be used as delimiter
 *
 * @~german
 * @brief Definiert das Trennsymbol zwischen den einzelnen Zeilen
 *
 * @param delimiterSign Zeichen, welches als Trennsymbol genutzt werden soll 
 *
 */

void gnublin_csv::delimiterRow(string delimiterSign) {
	delimiterRowSign = delimiterSign;
}


//********************** delimiterColumn ************************************
/**
 *
 * @~english
 * @brief Specifies the delimiter of each column
 *      
 * @param delimiterSign Sign which should be used as delimiter
 *
 * @~german
 * @brief Definiert das Trennsymbol zwischen den einzelnen Spalten
 *
 * @param delimiterSign Zeichen, welches als Trennsymbol genutzt werden soll 
 *
 */

void gnublin_csv::delimiterColumn(char delimiterSign) {
	delimiterColumnSign = delimiterSign;
}


//********************** delimiterField *************************************
/**
 *
 * @~english
 * @brief Specifies the delimiter of each field
 *      
 * @param delimiterSign Sign which should be used as delimiter
 *
 * @~german
 * @brief Definiert das Trennsymbol zwischen den einzelnen Feldern
 *
 * @param delimiterSign Zeichen, welches als Trennsymbol genutzt werden soll 
 *
 */

void gnublin_csv::delimiterField(char delimiterSign) {
	delimiterFieldSign = delimiterSign;
}


//********************** overloaded delimiterField **************************
/**
 *
 * @~english
 * @brief Set no delimiter for each field
 *      
 * @~german
 * @brief Kein Zeichen wird als Trennsymbol definiert
 *
 */

void gnublin_csv::delimiterField() {
	delimiterFieldSign = '\0';
}
/*
template <class T>
string NumberToString ( T Number )
{
  ostringstream ss;
  ss << Number;
  return ss.c_str();
}
*/







