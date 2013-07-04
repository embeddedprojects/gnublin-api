#include "gnublin.h"
 
int main()
{
   gnublin_module_dogm dogm;
 
   dogm.init();
   dogm.setRsPin(14);
   dogm.setCS(11);
 
   dogm.offset(2);
   dogm.print((char*)"Hallo Welt"); //(char*) ist ein cast, damit der compiler weiß, dass "Hallo Welt" als char* interpretiert werden soll, und nicht als string!
 
   sleep(2);
 
   dogm.clear();
   dogm.print((char*)"Zeile 1", 1);
   dogm.print((char*)"Zeile 2", 2);
   dogm.shift(5);
 
   sleep(2);
 
   dogm.returnHome();
   dogm.clear();
   dogm.print((char*)"Zeile 1, Offset 2", 1, 2);
 
   sleep(2);
 
   dogm.controlDisplay(0,1,0);
}
