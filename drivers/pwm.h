#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

//***************************************************************************
// Class for creating pwm signals
//***************************************************************************

/**
* @class gnublin_pwm
* @~english
* @brief Class for creating pwm signals
*
* This class manages the duty-cycle and the clock divider of a pwm signal.
* @~german
* @brief Klasse zum Erstellen von pwm Signalen
*
* Diese Klasse ermöglicht das Einstellen des Duty-Cycle und das Einstellen eines Clock Teilers.
*/

using namespace std;

class gnublin_pwm{
  public:
    gnublin_pwm();
		void setValue(float v);
		void setClock(int num);
};





