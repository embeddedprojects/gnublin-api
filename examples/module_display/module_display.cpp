#include "gnublin.h"
 
int main()
{
   gnublin_module_dogm dogm;
 
   dogm.setRsPin(14);
   dogm.setCS(11);
 
   dogm.offset(2);
   dogm.print("Hallo Welt");
 
   sleep(2);
 
   dogm.clear();
   dogm.print("Zeile 1", 1);
   dogm.print("Zeile 2", 2);
   dogm.shift(5);
 
   sleep(2);
 
   dogm.returnHome();
   dogm.clear();
   dogm.print("Zeile 1, Offset 2", 1, 2);
 
   sleep(2);
 
   dogm.controlDisplay(0,1,0);
}
