#ifndef NOTE_H
#define NOTE_H


#include "display.h"
#include "defines.h"

using namespace std;
class Note {



public:
	Note();	
	void writeTitle(unsigned char input);
	void writeWho(unsigned char input);
	void writeDescription(unsigned char input);
	void writeTime(unsigned char input);

	void deleteTitle();
	void deleteWho();
	void deleteDescription();
	void deleteTime();

	void increase_pos_title(int a);
	void increase_pos_who(int a);
	void increase_pos_description(int a);
	void increase_pos_time(int a);

	int cursor_pos_title;
	int cursor_pos_who;
	int cursor_pos_description;
	int cursor_pos_time;

	char Title[(LINES-1)*ROWS];
	char Who[(LINES-1)*ROWS];
	char Description[(LINES-1)*ROWS];
	char Time[(LINES-1)*ROWS];
};
#endif
