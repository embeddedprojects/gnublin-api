#include <gnublin.h>
#define ADDR 0x20



int c,hflag;
int slave_address = ADDR;
int xpos=0;
int ypos=0;
char *lcdstring = (char *)"";
int clear=0;
int init=0;
int blink=0;
int cursor=0;
int setdisplay=0;
int setstring=1;
int jasonflag=0;

using namespace std;
gnublin_module_lcd lcd;

void parse_opts(int argc, char **argv){
	while((c = getopt(argc,argv,"hja:o:l:idw:B:u:")) != -1){
		switch(c){
			case 'h' : hflag = 1;                               	break;	/* help */
			case 'a' : slave_address = strtol(optarg,NULL,16);	break;
			case 'o' : xpos = atoi(optarg);			    	break;
			case 'j' : jasonflag = 1;				break;
			case 'l' : ypos = atoi(optarg);			    	break;
			case 'w' : lcdstring = optarg; setstring=1;	    	break;
			case 'i' : init = 1; 				    	break;
			case 'd' : clear=1;				    	break;
			case 'B' : blink=atoi(optarg); setdisplay = 1;	    	break;
			case 'u' : cursor=atoi(optarg);	setdisplay = 1;	    	break;
		}

	}
	if (hflag | (argc<=1)){
		printf("This program is designed, to easily interact with a 4x20 Display connected to the GNUBLIN.\n\n"),
		printf("-h Show this help\n"
			"-f <device> Specify the i2c-device.default=/dev/i2c-1\n"
			"-j Convert output to json format.\n"
			"-a <I2C-address> Specify the display module's I2C-address.default=0x20\n"
			"-i Initialize the Display\n"
			"-o <column> Moves the cursor to the Position X (0-19)\n"
			"-l <line> Moves the cursor to the Line Y (1-4)\n"
			"-w <string> Prints the String to the Display\n"
			"-d clears the Display\n"
			"-B let the cursor blink(1/0)\n"
			"-u Shows the cUrsor(_)(1/0)\n\n"
			"Examples:\n\n"
			"%s -a 0x20 -i\n"
			"%s -a 0x20 -d -B 1 -o 3 -l 2 -w \"Hello World\"\n", argv[0], argv[0]);
		exit(1);
	}
}


int main (int argc, char **argv) {

	parse_opts(argc, argv);
	lcd.setAddress(slave_address);
	
	if(init){
		if(!lcd.init()){
			cout << lcd.getErrorMessage();
			return -1;
		}
		if(!lcd.setcursor(1, 1)){
			return -1;
		}
		if(!lcd.string("embedded-projects")){
			return -1;
		}
		if(!lcd.setcursor(2, 4)){
			return -1;
		}
		if(!lcd.string("GNUBLIN-LCD")){
			return -1;
		}
		if(!lcd.setcursor(3, 2)){
			return -1;
		}
		if(!lcd.string("www.gnublin.org")){
			return -1;
		}
		if(!lcd.setcursor(4, 4)){
			return -1;
		}
		if(!lcd.string("Version 0.3")){
			return -1;
		}
	}
	if(clear){
		lcd.clear();
	}
	if(xpos > 0 || ypos > 0){
		lcd.setcursor(ypos, xpos);
	}
	if(setdisplay){
		lcd.setdisplay(cursor,blink);
	}
	if(setstring){
		lcd.string(lcdstring);
	}
}






