#include "driver.h"

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
gnublin_driver::gnublin_driver() 
{
	error_flag=false;
        ErrorMessage="";
}

//------------------error messaging------------------
/** @~english
* @brief Called by the constructors to initialize class variables.
*
* @param message String contents that describe the error.
* @return failure: -1
*
* @~german
* @brief Wird von den Konstruktoren der Klasse Variablen zu initialisieren.
*
* @param message String Inhalte, die den Fehler beschreiben.
* @return failure: -1
*
*/
int gnublin_driver::setErrorMessage(std::string message)
{
   ErrorMessage=message;
   error_flag=true; 
   return -1; 
}

//------------------error messaging------------------
/** @~english
* @brief Resets the error_flag and other associated details.
*
* @~german
* @brief Setzt die error_flag und anderen damit verbundenen Informationen.
*
*/
void gnublin_driver::clearError()
{
   ErrorMessage="";
   error_flag=false; 
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
bool gnublin_driver::fail(){
	return error_flag;
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
const char *gnublin_driver::getErrorMessage(){
	return ErrorMessage.c_str();
}

