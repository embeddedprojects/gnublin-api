#include "gnublin.h"
 
int main()
{
   gnublin_i2c i2c;
/* 
  //send value  
   i2c.setAddress(0x60);
 
   unsigned char buffer[8]={0x0f,0xff, 
              0x07,0xff, 
              0x07,0xff,
              0x0f,0xff};
              
   i2c.send(buffer,8);
   */

/*
  unsigned char buffer[1];
  gnublin_gpio gpio;

  gpio.pinMode(14,OUTPUT);

  gpio.digitalWrite(14,HIGH);

  i2c.setAddress(0x60);
 
  buffer[0]=0x61;
  i2c.send(buffer,1);

  gpio.digitalWrite(14,LOW); //LDAC to low

  buffer[0]=0x62; //set to new address 0x61
  i2c.send(buffer,1);

  buffer[0]=0x67; //set to new address 0x61
  i2c.send(buffer,1);
*/
}
