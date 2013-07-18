#ifndef DISPLAYBUF_H
#define DISPLAYBUF_H

#include "display.h"
#include "defines.h"

using namespace std;

class DisplayBuffer {
	int cursorbuf;
	unsigned char displaybuf[LINES*ROWS];
	Display lcdisplay;
public:
	DisplayBuffer();
	void setText(int line, int row, const char *text);
	void setCursor(int pos);
	void printOnDisplay();
	void flushBuffer();
};
#endif
