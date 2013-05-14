#include "gnublin.h"
 
int main()
{
   gnublin_module_lm75 lm75;
 
 
   lm75.setAddress(0x4f);
 
   printf("Temperature %i \n", lm75.getTemp());
   printf("Raw Value %i \n",lm75.getValue());
   printf("Temperature Float Value %f \n",lm75.getTempFloat());
 
}
