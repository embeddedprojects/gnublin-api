#include "serial.h"

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
