#include <fstream>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <stdarg.h>
#include <sstream>

//***************************************************************************
// Class for creating csv files
//***************************************************************************

/**
* @class gnublin_csv
* @~english
* @brief Class for creating csv files
*
* This class manages creating of csv files und writing content to them.
* @~german
* @brief Klasse zum Erstellen von csv Dateien
*
* Diese Klasse ermöglicht das Erstellen der Dateien sowie das Schreiben von Inhalten.
*/

using namespace std;

class gnublin_csv{
  public:
    gnublin_csv();
		gnublin_csv(string new_filename);
  	int open(string new_filename);
		int addRow(int quantity, ...);
		void close();
		void delimiterRow(string delimiterSign);
	  void delimiterColumn(char delimiterSign);
  	void delimiterField(char delimiterSign);
		void delimiterField();
		template <typename T>
		string NumberToString(T Number) {
  		string str;
			stringstream ss;
  		ss << Number;
			str = ss.str();
  		return ss.str();
		}



	private:
		string filename;
		bool user_file_handle;
		string delimiterRowSign;
		char delimiterColumnSign;
		char delimiterFieldSign;
};



