#include "gnublin.h"
 
int main()
{
   gnublin_i2c i2c;
 
   i2c.setAddress(0x42);
 
   unsigned char buffer[8];
   unsigned char RxBuf[8];
 
   buffer[0]=0x22;
 
   i2c.send(buffer,5);
   i2c.send(0x12, buffer, 2);   //sende 2 byte aus buffer an Register-Adresse 0x12
 
   i2c.receive(RxBuf, 3);       // lese 3 bytes und speichere sie in RxBuf
   i2c.receive(0x23, RxBuf, 3);  // lese von Register-Adresse 0x23 3 bytes und speichere sie in RxBuf 
 
}
