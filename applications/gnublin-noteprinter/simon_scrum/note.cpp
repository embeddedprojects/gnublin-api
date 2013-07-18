#include "note.h"
using namespace std;
Note::Note(){
	cursor_pos_title=0;
	cursor_pos_who=0;
	cursor_pos_description=0;
	cursor_pos_time=0;
	
	for(int i=0; i < (LINES-1)*ROWS; i++){
		Title[i]=' ';
		Who[i]=' ';
		Description[i]=' ';
		Time[i]=' ';
	}
}

void Note::writeTitle(unsigned char input){
	Title[cursor_pos_title]=input;
	cursor_pos_title++;
	if (cursor_pos_title >= (LINES-1)*ROWS){
		cursor_pos_title=0;
	}
}

void Note::writeWho(unsigned char input){
	Who[cursor_pos_who]=input;
	cursor_pos_who++;
	if (cursor_pos_who >= (LINES-1)*ROWS){
		cursor_pos_who=0;
	}

}

void Note::writeDescription(unsigned char input){
	Description[cursor_pos_description]=input;
	cursor_pos_description++;
	if (cursor_pos_description >= (LINES-1)*ROWS){
		cursor_pos_description=0;
	}

}

void Note::writeTime(unsigned char input){
	Time[cursor_pos_time]=input;
	cursor_pos_time++;
	if (cursor_pos_time >= (LINES-1)*ROWS){
		cursor_pos_time=0;
	}

}


void Note::deleteTitle(){
	cursor_pos_title--;
	if(cursor_pos_title <0){
	cursor_pos_title = 0;
	}
	Title[cursor_pos_title]=' ';	
}

void Note::deleteWho(){
	cursor_pos_who--;
	if(cursor_pos_who <0){
	cursor_pos_who = 0;
	}
	Who[cursor_pos_who]=' ';
}

void Note::deleteDescription(){
	cursor_pos_description--;
	if(cursor_pos_description <0){
	cursor_pos_description = 0;
	}
	Description[cursor_pos_description]=' ';
}

void Note::deleteTime(){
	cursor_pos_time--;
	if(cursor_pos_time <0){
	cursor_pos_time = 0;
	}
	Time[cursor_pos_time]=' ';
}

void Note::increase_pos_title(int a){
	if(cursor_pos_title+a >=0 && cursor_pos_title+a < (LINES-1)*ROWS){
		cursor_pos_title=cursor_pos_title+a;
	}
}

void Note::increase_pos_who(int a){
	if(cursor_pos_who+a >=0 && cursor_pos_who+a < (LINES-1)*ROWS){
		cursor_pos_who=cursor_pos_who+a;
	}
}

void Note::increase_pos_description(int a){
	if(cursor_pos_description+a >=0 && cursor_pos_description+a < (LINES-1)*ROWS){
		cursor_pos_description=cursor_pos_description+a;
	}
}

void Note::increase_pos_time(int a){
	if(cursor_pos_time+a >=0 && cursor_pos_time+a < (LINES-1)*ROWS){
		cursor_pos_time=cursor_pos_time+a;
	}
}
