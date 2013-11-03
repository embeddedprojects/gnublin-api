#include "../../gnublin.h"



#define ADDR 0x60

using namespace std;

unsigned int position = 0;
int c, hflag;
int slave_address = ADDR;
char buffer[128];
char rx_buf[128];
int json_flag = 0;
int brute_flag = 0;
unsigned int irun = 15;
unsigned int ihold = 0;
unsigned int vmax = 8;
unsigned int vmin = 0;
unsigned int stepmode = 11;
unsigned int accshape = 0;

int new_address = -1;


gnublin_module_step Motor;


void parse_opts(int argc, char** argv) {

  while((c = getopt(argc, argv, "hjp:f:a:bi:d:x:n:o:cs:")) != -1) {
    switch(c) {
      case 'h' :
        hflag = 1;
        break;                              /* help */
      case 'p' :
        Motor.setMotorParam();
        Motor.getFullStatus1();
        Motor.runInit();
        Motor.drive(atoi(optarg));
        break;
      case 'a' :
        Motor.setAddress(strtol(optarg, NULL, 16));
        break;
      case 'j' :
        json_flag = 1;
        break;
      case 'b' :
        brute_flag = 1;
        break;
      case 'i' :
        Motor.setIrun(atoi(optarg));
        break;
      case 'd' :
        Motor.setIhold(atoi(optarg));
        break;
      case 'x' :
        Motor.setVmax(atoi(optarg));
        break;
      case 'n' :
        Motor.setVmin(atoi(optarg));
        break;
      case 'o' :
        Motor.burnNewAddress(strtol(optarg, NULL, 16));
        break;
      case 's' :
        Motor.setStepMode(atoi(optarg));
        break;
      case 'c' :
        Motor.setAccShape(1);
        break;
    }

  }
  if(hflag | (argc == 1)) {
    printf("This program is designed, to easily interact with a stepper-motor connected to the GNUBLIN.\n\n"
           "-h Show this help\n"
           "-j Convert output to json format.\n"
           "-b show output in raw format.\n"
           "-a <I2C-address> Specify the stepper modules I2C-address.default=0x60\n"
           "-i <Irun> Specify the Irun parameter (0-15)\n"
           "-d <Ihold> Specify the iholD parameter (0-15)\n"
           "-x <vmax> Specify the vmaX parameter (0-15)\n"
           "-n <vmin> Specify the vmiN parameter (0-15)\n"
           "-s <Stepmode> StepMode 0=Halfstep, 4=1/4µStep, 8=1/8µStep, 11=1/16µStep\n"
           "-c Sets the AccShape Bit to 1 - Motor will not accelerate -> constant vmin speed\n"
           "-p <Position> Specify the desired position. Hint: The Position argument should be the last one\n"
           "-o <new_address> burns the new I2C Address to the controller\n\n"
           "Examples:\n\n"
           "\tDrive the motor to position 3000 and use I2C-address 0x60:\n"
           "\t%s -a 0x60 -p 3000\n"
           "\tA complete rotation is position 3200, two rotations 6400 and so on.\n", argv[0]);
    exit(1);
  }
}



int main(int argc, char** argv) {
  Motor.setAddress(slave_address);
  parse_opts(argc, argv);

}

