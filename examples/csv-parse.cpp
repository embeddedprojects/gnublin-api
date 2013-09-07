#include "gnublin.h"
#include <iostream>



int main() {
	
	string str1, str2, str3;
	str1 = "Erster Eintrag";
	str2 = "Zweiter Eintrag";
	str3 = "Dritter Eintrag";


	//Open file with open(string filename) and customized delimiters
	gnublin_csv csv;
	
	csv.open("test.csv");
	
  csv.delimiterRow("\r\n\n");	// Default: "\r\n"
  csv.delimiterField('\0');		// Default: '"'
  csv.delimiterColumn('\t'); 	// Default: ';'
	

	csv.addRow(3, str1.c_str(), str2.c_str(), str3.c_str());
	csv.addRow(3, "Ja", "mal", "schaun");
	csv.addRow(3, "Noch", "eine", "Reihe");
	



	//Open file with constructor and default delimiters
	gnublin_csv csv2("test2.csv");

  csv2.addRow(3, str1.c_str(), str2.c_str(), str3.c_str());
  csv2.addRow(3, "Ja", "mal", "schaun");
  csv2.addRow(3, "Noch", "eine", "Reihe");

	//Add integer and float variables
	gnublin_csv csv3("test3.csv");
	
	int iWert1 = 1, iWert2 = 2, iWert3 = 4;
	float fWert1 = 1.18283811, fWert2 = 1231232.2352352, fWert3 = 213.003;	
	double dWert1 = 2.3123123123, dWert2 = 9992822223.21, dWert3 = 60219.203;

	csv3.addRow(3, (csv3.NumberToString(iWert1)).c_str(), (csv3.NumberToString(iWert2)).c_str(), (csv3.NumberToString(iWert3)).c_str());
	csv3.addRow(3, (csv3.NumberToString(fWert1)).c_str(), (csv3.NumberToString(fWert2)).c_str(), (csv3.NumberToString(fWert3)).c_str());
	csv3.addRow(3, (csv3.NumberToString(dWert1)).c_str(), (csv3.NumberToString(dWert2)).c_str(), (csv3.NumberToString(dWert3)).c_str());

	return 0;
}
