#include "gnublin.h"
 
int main() {
  gnublin_module_relay relay;
 
  relay.setAddress(0x24);
 
  relay.switchPin(4, ON);
  sleep(2);
  relay.switchPin(4, OFF);
}
