#include "gnublin.h"
#include <ctime>
using namespace std;

gnublin_module_pca9555 minutes, hours;

time_t t;
struct tm *ts;
bool sec=true;
char *tz = (char*)"TZ=NFT-1DFT"; //middle european time zone
    	


int writeHours(int hour){
	unsigned char h1, h2;
	h1=0;
	h2=0;
	if(hour>12) //24 -> 12 h format
		hour-=12;
	if(hour<=8){ //use port 0
		h1=1;
		h1<<=(hour-1);
	}
	else{	//use port 1
		h2=1;
		h2<<=(hour-8-1);
	}
	//write hour pins
	if(hours.writePort(0,h1)<0){ 
		cout << "ERROR: write port 0 of hours" << endl;
		cout << hours.getErrorMessage() << endl;
		exit(-1);
	}
	if(hours.writePort(1,h2)<0){
		cout << "ERROR: write port 1 of hours" << endl;
		cout << hours.getErrorMessage() << endl;
		exit(-1);
	}
	
	return 0;
}

int writeMinutes(int minute){
	unsigned char m1, m2,c;
	
	m1=0;
	m2=0;
	if(sec==true){ //blink the single minute led
		if(minute<5){ //show only led 0 if minues are < 5
			m2=1;
			m2<<=(3);
		}
		else if((minute/5)<=8){	//use port 0
			m1=1;
			m1<<=((minute/5)-1);
		}
		else{	//use port 1
			m2=1;
			m2<<=((minute/5)-8-1);
		}
		sec=false;
	}
	else 
		sec=true;
		
	c=(pow(2,(minute%5))-1); //get the non full 5 minutes
	m2=m2|(c<<=4);		 //print 'em
	
	if(minutes.writePort(0,m1)<0){
		cout << "ERROR: write port 0 of minutes" << endl;
		cout << minutes.getErrorMessage() << endl;
		exit(-1);
	}
	if(minutes.writePort(1,m2)<0){
		cout << "ERROR: write port 1 of minutes" << endl;
		cout << minutes.getErrorMessage() << endl;
		exit(-1);
	}
	
	return 0;
	
}

int writeClock(){
	
	t = time(NULL);
	ts = localtime(&t);
	//cout << ts->tm_mday << '.' << ts->tm_mon+1 << '.' << ts->tm_year+1900 << " - " << ts->tm_hour << ':' <<ts->tm_min << ':' << ts->tm_sec << endl;
	writeHours(ts->tm_hour); //call function to write the hours
	writeMinutes(ts->tm_min); //call function to write the minutes

	return 0;
}

int main(int argc, char* argv[]){

        unsigned char c1,c2;
        int i ;
        
        //set the right timezone
        putenv(tz);
	tzset();
        
        //set right addresses
        minutes.setAddress(0x20);
        hours.setAddress(0x27);

	//set pins as output
        if(minutes.portMode(0,OUTPUT)<0){
        	cout << "ERROR: set port 0 of minutes as output"<<endl;
                cout << minutes.getErrorMessage() << endl;
                exit(-1);
        }
        if(minutes.portMode(1,OUTPUT)<0){
        	cout << "ERROR: set port 1 of minutes as output" << endl;
                cout << minutes.getErrorMessage() << endl;
                exit(-1);
        }
        
        if(hours.portMode(0,OUTPUT)<0){
        	cout << "ERROR: set port 0 of hours as output" << endl;
                cout << hours.getErrorMessage() << endl;
                exit(-1);
        }
        if(hours.portMode(1,OUTPUT)<0){
        	cout << "ERROR: set port 1 of minutes as output" << endl;
                cout << hours.getErrorMessage() << endl;
                exit(-1);
        }
        if(argc==1){
        	while(1){
			writeClock();
        		sleep(1);
        	}
        }
        else{
        	while(1){
        		time(&t);
			c1=1;
			c2=0;
		        for (i=0;i<=15;i++){
		        	
		        	minutes.writePort(0,c1);
		        	minutes.writePort(1,c2);
		        	hours.writePort(0,c1);
		        	hours.writePort(1,c2);
		        	if(c1==0x80)
		        		c2=1;
		        	else 
		        		c2<<=1;
		        	c1<<=1;
		        	usleep(100000);
		        }
		}
        
        }
        return 0;
        
}
