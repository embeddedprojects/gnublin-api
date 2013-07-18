#include "keyboard.h"
#include "application.h"
#include "screens.h"
#include "note.h"
#include <usb.h>
#include <iostream>

using namespace std;


char buffer[8];

Note Scrumnote;
Screens Screen;

int main(){
	

	State DisplayState;
	DisplayState = MAIN;
	usb_dev_handle *usbdev = keyboard_init();
	unsigned char pressed_key;


	while(1){
	
	// get the keyboard input and parse which key is pressed
		getInput(usbdev, buffer);
		pressed_key=application_hid_to_ascii(buffer);


	  // Application Control
	  switch(DisplayState){

		case MAIN:
			printf("State == MAIN\n buffer: %d", application_hid_to_ascii(buffer));

			switch(pressed_key){
				case KEY_TAB: DisplayState=TITLE; break;
				default:
					Screen.MainScreen();
			}
		break;

		case TITLE:			
			printf("State == TITLE\n buffer: %d", application_hid_to_ascii(buffer));

			switch(pressed_key){
				case KEY_TAB: DisplayState=WHO; break;
				case KEY_BACKSPACE: 
					Scrumnote.deleteTitle();
					Screen.TitleScreen();
				break;
				case KEY_ENTER:
					if(Scrumnote.cursor_pos_title < 20){
						Scrumnote.cursor_pos_title = 20;
					}
					else if (Scrumnote.cursor_pos_title < 40){
						Scrumnote.cursor_pos_title = 40;
					}
					else if (Scrumnote.cursor_pos_title < 80){
						Scrumnote.cursor_pos_title = 0;
					}
				break;
				case KEY_ARROW_RIGHT:
					Scrumnote.increase_pos_title(1);
				break;
				case KEY_ARROW_LEFT:
					Scrumnote.increase_pos_title(-1);
				break;
				case KEY_ARROW_DOWN:
					Scrumnote.increase_pos_title(20);
				break;
				case KEY_ARROW_UP:
					Scrumnote.increase_pos_title(-20);
				break;
				case 0: Screen.TitleScreen(); break;
				default:
				Scrumnote.writeTitle(pressed_key);
				Screen.TitleScreen();
				cout << "Scrumnote.Title:" << "'" << Scrumnote.Title << "'" << endl;
			}
		
			
		break;

		case WHO:
			printf("State == WHO\n");
			switch(pressed_key){
				case KEY_TAB: DisplayState=DESCRIPTION; break;
				case KEY_BACKSPACE: 
					Scrumnote.deleteWho();
					Screen.WhoScreen();
				break;
				case KEY_ENTER:
					if(Scrumnote.cursor_pos_who < 20){
						Scrumnote.cursor_pos_who = 20;
					}
					else if (Scrumnote.cursor_pos_who < 40){
						Scrumnote.cursor_pos_who = 40;
					}
					else if (Scrumnote.cursor_pos_who < 80){
						Scrumnote.cursor_pos_who = 0;
					}
				break;
				case KEY_ARROW_RIGHT:
					Scrumnote.increase_pos_who(1);
				break;
				case KEY_ARROW_LEFT:
					Scrumnote.increase_pos_who(-1);
				break;
				case KEY_ARROW_DOWN:
					Scrumnote.increase_pos_who(20);
				break;
				case KEY_ARROW_UP:
					Scrumnote.increase_pos_who(-20);
				break;
				case 0: Screen.WhoScreen(); break;
				default:
				Scrumnote.writeWho(pressed_key);
				Screen.WhoScreen();
			}
		break;

		case DESCRIPTION:
			printf("State == DESCRIPTION\n");
			switch(pressed_key){
				case KEY_TAB: DisplayState=TIME; break;
				case KEY_BACKSPACE: 
					Scrumnote.deleteDescription();
					Screen.DescriptionScreen();
				break;
				case KEY_ENTER:
					if(Scrumnote.cursor_pos_description < 20){
						Scrumnote.cursor_pos_description = 20;
					}
					else if (Scrumnote.cursor_pos_description < 40){
						Scrumnote.cursor_pos_description = 40;
					}
					else if (Scrumnote.cursor_pos_description < 80){
						Scrumnote.cursor_pos_description = 0;
					}
				break;
				case KEY_ARROW_RIGHT:
					Scrumnote.increase_pos_description(1);
				break;
				case KEY_ARROW_LEFT:
					Scrumnote.increase_pos_description(-1);
				break;
				case KEY_ARROW_DOWN:
					Scrumnote.increase_pos_description(20);
				break;
				case KEY_ARROW_UP:
					Scrumnote.increase_pos_description(-20);
				break;
				case 0: Screen.DescriptionScreen(); break;
				default:
				Scrumnote.writeDescription(pressed_key);
				Screen.DescriptionScreen();
			}
		break;

		case TIME:
			printf("State == TIME\n");
			switch(pressed_key){
				case KEY_TAB: DisplayState=PRINT; break;
				case KEY_BACKSPACE: 
					Scrumnote.deleteTime();
					Screen.TimeScreen();
				break;
				case KEY_ENTER:
					if(Scrumnote.cursor_pos_time < 20){
						Scrumnote.cursor_pos_time = 20;
					}
					else if (Scrumnote.cursor_pos_time < 40){
						Scrumnote.cursor_pos_time = 40;
					}
					else if (Scrumnote.cursor_pos_time < 80){
						Scrumnote.cursor_pos_time = 0;
					}
				break;
				case KEY_ARROW_RIGHT:
					Scrumnote.increase_pos_time(1);
				break;
				case KEY_ARROW_LEFT:
					Scrumnote.increase_pos_time(-1);
				break;
				case KEY_ARROW_DOWN:
					Scrumnote.increase_pos_time(20);
				break;
				case KEY_ARROW_UP:
					Scrumnote.increase_pos_time(-20);
				break;
				case 0: Screen.TimeScreen(); break;
				default:
				Scrumnote.writeTime(pressed_key);
				Screen.TimeScreen();
			}
		break;

		case PRINT:
			printf("State == PRINT\n");
			switch(pressed_key){
				case KEY_TAB: DisplayState=MAIN; break;
				default:
				Screen.PrintScreen();
			}
		break;

	    }

	sleep(0.2);
	}
keyboard_close(usbdev);
}

