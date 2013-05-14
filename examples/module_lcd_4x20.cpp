#include "gnublin.h"
 
char *version = (char *) "0.3";
 
int main(){
  gnublin_module_lcd lcd;
 
  lcd.init();
  lcd.setcursor(1, 1);
  lcd.string("embedded-projects");
  lcd.setcursor(2, 4);
  lcd.string("GNUBLIN-LCD");
  lcd.setcursor(3, 2);
  lcd.string("www.gnublin.org");
  lcd.setcursor(4, 4);
  lcd.string("Version ");
  lcd.string(version);
 
}
