#include "gnublin.h"
 
int main(){
  gnublin_module_pca9555 pca;
  pca.setAddress(0x22);      // Portexpander befindet sich bei Adresse 0x20
  pca.pinMode(3, OUTPUT);    //Port 3 als Ausgang festlegen
 
  while(1) {
    pca.digitalWrite(3, HIGH);   // an
    sleep(1);                    // 1 Sekunde warten
    pca.digitalWrite(3, LOW);    // aus
    sleep(1);                    // wieder 1 Sekunde warten
  }
}
