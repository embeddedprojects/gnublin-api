#include "screens.h"
void Screens::MainScreen(){
	lcdbuf.flushBuffer();
	lcdbuf.setText(1,1,"Welcome to ScrumTXT");
	lcdbuf.setText(2,1,"Press TAB to switch");
	lcdbuf.setText(3,1,"to the next Screen");
	lcdbuf.setText(4,1,"<F4> print <F5> save");
	lcdbuf.printOnDisplay();
}

void Screens::TitleScreen(){
	lcdbuf.flushBuffer();
	lcdbuf.setText(1,1,"Title:");
	lcdbuf.setText(2,1,Scrumnote.Title);
	lcdbuf.setCursor(Scrumnote.cursor_pos_title);
	lcdbuf.printOnDisplay();
}

void Screens::WhoScreen(){
	lcdbuf.flushBuffer();
	lcdbuf.setText(1,1,"Who:");
	lcdbuf.setText(2,1,Scrumnote.Who);
	lcdbuf.setCursor(Scrumnote.cursor_pos_who);
	lcdbuf.printOnDisplay();
}

void Screens::DescriptionScreen(){
	lcdbuf.flushBuffer();
	lcdbuf.setText(1,1,"Description:");
	lcdbuf.setText(2,1,Scrumnote.Description);
	lcdbuf.setCursor(Scrumnote.cursor_pos_description);
	lcdbuf.printOnDisplay();
}

void Screens::TimeScreen(){
	lcdbuf.flushBuffer();
	lcdbuf.setText(1,1,"Time:");
	lcdbuf.setText(2,1,Scrumnote.Time);
	lcdbuf.setCursor(Scrumnote.cursor_pos_time);
	lcdbuf.printOnDisplay();
}

void Screens::PrintScreen(){
	lcdbuf.flushBuffer();
	lcdbuf.setText(1,1,"Print:");
	lcdbuf.printOnDisplay();

}
