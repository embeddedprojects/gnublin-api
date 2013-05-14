#include "gnublin.h"
 
int main()
{
   gnublin_gpio gpio;
 
   gpio.pinMode(3,INPUT);
 
   while(1){
     if(gpio.digitalRead(3))
     {
        printf("GPIO is set /n");
     }
     sleep(2);
   }
}
