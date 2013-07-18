#ifndef SCREEN_H
#define SCREEN_H

#include "display_buffer.h"
#include "note.h"

extern Note Scrumnote;

class Screens {
	DisplayBuffer lcdbuf;
public:
	void MainScreen();
	void TitleScreen();
	void WhoScreen();
	void DescriptionScreen();
	void TimeScreen();
	void PrintScreen();
};
#endif
