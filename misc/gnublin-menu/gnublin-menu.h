#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> /* O_RDONLY=0 O_RDWR=2 ADC read/select */
#include <unistd.h> /* close() read() write() ADC Read/select */
#include <sys/types.h> /* Fuer dogm display print() */

#define DOGM "gnublin-dogm"  //Fuer Display

#ifndef FILE_D
#define FILE_D

struct menu
{
    int menu_type;
    int pos;
    char top[16];
    char bottom[16];
    void (*fp)(void);
    struct menu *mother;
    struct menu *child[16];
} menu_struct;

struct menu *root;
struct menu *zeiger;

struct menu* MainMenu(char*);
struct menu* SubMenu(struct menu*, char*);

void print(struct menu*);
int DisplayRow1(char*);
int DisplayRow2(char*);
void run(void);
void prev(void);
void ok(void);
void next(void);
void RegisterMenu(struct menu*, void (*)(void)); //?????
void RegisterNumberOfButtons(int);
void RegisterMenuButtons(int, int, int);

int event(void);
//int selectADC(int);
int selectADC( int chanel, int fd );

int getADC(void);

// Konstanten:
extern int button_anzahl;
extern int button_prev;
extern int button_ok;
extern int button_next;

extern int fd;


#endif
