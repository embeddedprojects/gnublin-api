#include "gnublin.h"

int main(){
	gnublin_module_dac dac;

	dac.write(0, 4095);
	dac.write(1, 2048);
	dac.write(2, 1024);
	dac.write(3, 0);
}
