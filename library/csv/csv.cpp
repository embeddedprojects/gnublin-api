#include "csv.h"

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







