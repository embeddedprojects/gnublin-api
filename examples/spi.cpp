#include "gnublin.h"
 
#define BOARD EXTENDED
 
int main()
{
   gnublin_spi spi;
 
   spi.setSpeed(10000);
   spi.setCS(14);
   unsigned char send_buffer[9] = "getValue";
   char buffer[9];   
   while(1){
     spi.send(send_buffer,8);
     sleep(2);
     spi.receive(buffer,8);
   }
}
