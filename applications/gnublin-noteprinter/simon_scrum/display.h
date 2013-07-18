#ifndef DISPLAY_H
#define DISPLAY_H

#include "gnublin.h"
#include "defines.h"

class Display {
	int old_cursor;
	gnublin_module_lcd lcd;
	int charcnt;
	int line;
	State currentState;
	unsigned char old_output[LINES*ROWS];

public:
	Display();
	void write(unsigned char output[], int cursorpos);
};
#endif
