#include "module_lcd.h"
#include "../include/includes.h"
#include "module_pca9555.cpp"

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
