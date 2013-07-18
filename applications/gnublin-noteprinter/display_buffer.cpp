#include "display_buffer.h"

DisplayBuffer::DisplayBuffer(){
	cursorbuf=0;
}

void DisplayBuffer::setText(int line, int row, const char *text){
	int x=0;

	if (line==1){
		cursorbuf=0+(row-1);
	}
	else if (line > 1 && line <= 4){
		cursorbuf=(line-1)*20+(row-1);
	}        
	for(int i=cursorbuf; i < (cursorbuf + strlen(text)) && i <= LINES*ROWS; i++){
		displaybuf[i]=text[x];
		x++;
	}
}

void DisplayBuffer::setCursor(int pos){
	cursorbuf=pos;
}

void DisplayBuffer::printOnDisplay(){
	lcdisplay.write(displaybuf,cursorbuf);
}

void DisplayBuffer::flushBuffer(){
	for(int i=0; i < LINES*ROWS; i++){
		displaybuf[i]=' ';	
	}	
}
