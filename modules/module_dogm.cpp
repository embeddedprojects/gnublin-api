#include "module_dogm.h"

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
