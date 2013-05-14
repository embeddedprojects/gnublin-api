#include "gnublin.h"
 
int main()
{
   gnublin_adc ad;
 
 
   while(1){
        printf("AD value %i \n",ad.getValue(1));
   }
}
