#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>
#include <stdio.h>

using namespace std;

#define KEY_ARROW_RIGHT 4
#define KEY_ARROW_LEFT 5
#define KEY_ARROW_DOWN 6
#define KEY_ARROW_UP 7
#define KEY_BACKSPACE 8
#define KEY_TAB 9
#define KEY_ENTER 10


unsigned char application_hid_to_ascii(char *buffer);
#endif
