#include "application.h"

unsigned char application_hid_to_ascii(char *buffer){

	int ascii;
	unsigned char output;


	if( buffer[2]>=4 && buffer[2] <=29){ //characters a-z
		if (buffer[0] == 2){
			ascii=buffer[2]-4+65;
		}
		else{		
			ascii=buffer[2]-4+97;
		}
		//switch z <=> y
		
		if (ascii == 121){ 
			ascii=122;
		}
		else if (ascii == 122){
			ascii=121;
		}		
		else if (ascii == 90){
			ascii=89;
		}
		else if (ascii == 89){
			ascii=90;
		}
		
		output=ascii;
		return output;
	}
	else if (buffer[2]>=30 && buffer[2] <=39){ // numbers 0-9
		if(buffer[2]==39){ // zero
			if (buffer[0] == 2){
				ascii=buffer[2]-39+61; // = !"$ etc.
			}
			else{
				ascii=buffer[2]+9;
			}			
		}
		else{
			if (buffer[0] == 2){
				ascii=buffer[2]-30+33;
			}
			else{
				ascii=buffer[2]+19;
			}
		}
		output=ascii;
		return output;
	}
	else if (buffer[2] ==40){ //Enter Key
		ascii=10;
		output=ascii;
		return output;

	}
	else if (buffer[2] ==42){ //Backspace
		ascii=8;
		output=ascii;
		return output;
	}
	else if (buffer[2] ==43){ //TAB
		ascii=KEY_TAB;
		output=ascii;
		return output;
	}
	else if (buffer[2] ==44){ // Space
		output=' ';
		return output;
	}
	else if (buffer[2] ==45){ // ?
		ascii=63;
		output=ascii;
		return output;
	}
	else if (buffer[2] == 47){ // ü
		if (buffer[0] == 2){
			ascii=154;
		}
		else ascii=129;
		
		output=ascii;
		return output;
	}
	else if (buffer[2] == 51){ // ö
		if (buffer[0] == 2){
			ascii=153;
		}
		else ascii=148;
		
		output=ascii;
		return output;
	}
	else if (buffer[2] == 52){ // ä
		if (buffer[0] == 2){
			ascii=142;
		}
		else ascii=132;
		
		output=ascii;
		return output;
	}
	else if (buffer[2] ==54){ // comma
		if (buffer[0] == 2){
			output=';';
		}
		else output=',';
		return output;
	}
	else if (buffer[2] ==55){ // dot
		if (buffer[0] == 2){
			output=':';
		}
		else output='.';
		return output;
	}
	else if (buffer[2] ==79){ // Arrow right
		ascii=KEY_ARROW_RIGHT;
		output=ascii;
		return output;
	}
	else if (buffer[2] ==80){ // Arrow left
		ascii=KEY_ARROW_LEFT;
		output=ascii;
		return output;
	}
	else if (buffer[2] ==81){ // Arrow down
		ascii=KEY_ARROW_DOWN;
		output=ascii;
		return output;
	}
	else if (buffer[2] ==82){ // Arrow up
		ascii=KEY_ARROW_UP;
		output=ascii;
		return output;
	}
	else{ // no key pressed
		ascii=0;
		output=ascii;
		return output;
	}
}
