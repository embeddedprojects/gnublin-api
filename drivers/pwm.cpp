#include "pwm.h"


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


