/*
 * Author: Felix Steinberger
 *
 * Display_controll v1.0
 *
 * Description:
 * This is an application to controll an DOGM162x-A LCD-Display with ST7036 controller.
 *
 * Before starting this application please export the GPIO_PIN (e.g. export GPIO_PIN=gpio11)
 * environment variable correspond with you choosen gpio pin on your
 * board(RS_PIN at display).
 *
 */

#include <gnublin.h>

using namespace std;

gnublin_module_dogm dogm;

int hflag = 0; 
int resetflag = 0;
int cursorflag = 0;
int shiftflag = 0;
int json_flag = 0;
int bare_flag = 0;
int pinflag = 0;
int csflag = 0;
int init_flag = 0;
int displayflag = 0;
int clearflag = 0;
int lineflag = 0;
int cursor_offset = 0;
int shift_val = 0;
int c, line_number = 0;
#if (BOARD == RASPBERRY_PI)
int pinnumber = 60;
int csnumber = 0;
#elif (BOARD == BEAGLEBONE_BLACK)
int pinnumber = 4;
int csnumber = 60;
#else
int pinnumber = 14;
int csnumber = 11;
#endif
char* string_display;


int error_msg(char* msg){
	if (json_flag) {
		printf("{\"error_msg:\" : \"%s not successful\", \"result\" : \"-1\",}\n", msg);
	}
	else if (bare_flag) {
	}
	else {
		printf("Error: %s not successful\n", msg);
	}
	exit (1);
}


void pars_opts(int argc, char **argv) {
	while((c = getopt(argc,argv,"bndjhw:l:o:s:g:ic:")) != -1){
                switch(c)
                {
                        case 'h' : hflag = 1;					break;                              /* help */
                        case 'w' : string_display = optarg; displayflag = 1;	break;
			case 'l' : line_number = atoi(optarg); lineflag = 1;	break;
                        case 'o' : cursor_offset = atoi(optarg); cursorflag = 1;break;
                        case 'n' : resetflag = 1;				break;
			case 'd' : clearflag = 1;				break;
                        case 's' : shift_val = atoi(optarg);shiftflag = 1;	break;
                        case 'j' : json_flag = 1;				break;
                        case 'g' : pinnumber =  atoi(optarg); pinflag = 1;	break;
                        case 'c' : csnumber =  atoi(optarg);  csflag = 1;	break;
                        case 'i' : init_flag = 1;				break;
			case 'b' : bare_flag = 1;				break;
                }

        }
	if (hflag | (argc<=1))
        {
                printf("This program was designed to easily interact with the dogm display.\n\n");            
                puts("-h Show this Help\n"
		"-w <STRING> write string to display\n"
		"-l <line> set string on line (1 = line 1; 2 = line 2)\n"
		"-j Convert Output to json Format\n"
		"-b Convert Output to bare format, no Status-Output\n"
		"-o <column> Set cursor to position(Start of line 1= 0; Start of line 2= 16)\n"
		"-n reset the display.\n"
		"-d delet the display\n"
		"-s <X> shift display <X> times(left shift = - ; right shift= +  \n"
		"-g <X> Use GPIO Pin X instead default GPIO Pin 14 (For RS Pin on DOGM Display)\n"
		"-i initialize the display\n"
		"-c <Y> Only initialize the spidev module with <Y> as Chipselect Pin.(default:<Y>=11)\n"
		"\n\nExamples:\nWrite Hello to the Display:\ngnublin-dogm -n -w \"Hello\"\n\n"
		"Write Hello to the Display connected with CS-Pin=18\ngnublin-dogm -n -w \"Hello\" -c 18\n\n"
		"Jump to the second Line with Cursor\ngnublin-dogm -l 2\n");

		exit(1);
               
        }
	
	if (pinflag){
		dogm.setRsPin(pinnumber);
		if (dogm.fail()){
			error_msg((char*)"setRS");
		}
	}
	if (csflag){
		dogm.setCS(csnumber);
		if (dogm.fail()){
			error_msg((char*)"setCsPin");
		}
	}
	if (init_flag){
		dogm.init();
		if (dogm.fail()){
			error_msg((char*)"init");
		}
	}
}




int main(int argc, char **argv) {
	pars_opts(argc, argv);
	
	if (resetflag) {
		dogm.clear();
		if (dogm.fail()){
			error_msg((char*)"clear");
		}
		dogm.returnHome();
		if (dogm.fail()){
			error_msg((char*)"returnHome");
		}
			
	}
	if (clearflag) {
		dogm.clear();
		if (dogm.fail()){
			error_msg((char*)"clear");
		}
	}
	if (shiftflag) {
		dogm.shift(shift_val);
		if (dogm.fail()){
			error_msg((char*)"shift");
		}
		if (json_flag) {
			printf("{\"Display_shift:\" : \"%i\", \"result\" : \"0\"}\n", shift_val);
		}
		else if (bare_flag) {
		}
		else {
			printf("Display shift --> %i \n", shift_val);
		}
	}
	if (cursorflag && !lineflag) {
		dogm.offset(cursor_offset);
		if (dogm.fail()){
			error_msg((char*)"offset");
		}
		if (json_flag) {
			printf("{\"cursor_offset:\" : \"%i\", \"result\" : \"0\"}\n", cursor_offset);
		}
		else if (bare_flag) {
		}
		else {
			printf("cursor offset --> %i \n", cursor_offset);
		}
	}
	if (lineflag && displayflag && !cursorflag){
		dogm.print(string_display, line_number);
		if (dogm.fail()){
			error_msg((char*)"print");
		}
		if (json_flag) {
			printf("{\"Display_print:\" : \"%s\", \"result\" : \"0\"}\n", string_display);
		}
		else if (bare_flag) {
		}
		else {
			printf("Line: %i \nDisplay print: %s \n", line_number, string_display);
		}
	}
	if (lineflag && displayflag && cursorflag){
		dogm.print(string_display, line_number, cursor_offset);
		if (dogm.fail()){
			error_msg((char*)"print");
		}
		if (json_flag) {
			printf("{\"Display_print:\" : \"%s\", \"result\" : \"0\"}\n", string_display);
		}
		else if (bare_flag) {
		}
		else {
			printf("Display print: %s \n", string_display);
		}
	}
	if (displayflag && !lineflag) {
		dogm.print(string_display);
		if (dogm.fail()){
			error_msg((char*)"print");
		}
		if (json_flag) {
			printf("{\"Display_print:\" : \"%s\", \"result\" : \"0\"}\n", string_display);
		}
		else if (bare_flag) {
		}
		else {
			printf("Display print: %s \n", string_display);
		}
	} 
}
