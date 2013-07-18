#include "display.h"
#include "defines.h"

using namespace std;


Display::Display(){
	lcd.init();
	lcd.setdisplay(0,1);
	line=1;
	charcnt=0;
}


void Display::write(unsigned char output[], int cursorpos){

	int cursor_changed = 0;
	charcnt=0;

	for(int i=0; i < LINES*ROWS; i++){

		if(old_output[i] != output[i]){ // if old Buffer != New Buffer, print the changed chars
			if(cursor_changed !=1){
				lcd.setdisplay(0,0);
				cursor_changed=1;
			}
			lcd.setcursor(line,charcnt);
			lcd.sendData(output[i]);
			
		}
		charcnt++;

		if (charcnt>=ROWS){
			if (line == LINES){
				line=0;
			}
			charcnt=0;
			line++;
		}
	}

        // set flashing cursor
	
	if(old_cursor != cursorpos){
		if(cursorpos < 20){
			lcd.setcursor(2, cursorpos);
		}
		else if (cursorpos < 40){
			lcd.setcursor(3, (cursorpos-20));
		}
		else if (cursorpos < 60){
			lcd.setcursor(4,(cursorpos-40));
		}
	}
	if(cursor_changed == 1){
		lcd.setdisplay(0,1);
	}
	
	// fill old buffer with the new buffer, set old cursor to actual cursor position
	old_cursor=cursorpos;

	for(int i=0; i < LINES*ROWS; i++){
	old_output[i]=output[i];
	}
}
