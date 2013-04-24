/*	Beispielhafter Code eines Users
		Autor: 		Julian
		Version: 	V1.0
		Datum: 22.03.13									*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gnublin-menu.h"	// Headerdatei wird eingebunden!

// Konstanten:
int button_anzahl = 3;
int button_prev = 0;
int button_ok = 1;
int button_next = 3;


void Screen_MinTemp(void);
void Screen_MaxTemp(void);
void Screen_Duration(void);
void Screen_ip(void);

int fd;

int main()
{
    struct menu *configuration, *network, *state;							//MainMenu
    struct menu *mintemp, *maxtemp, *start, *duration;				//Configuration
    struct menu *ipchoose, *ip, *netaddress, *gateway, *dns;	//Network
    struct menu *temperature, *cpu, *ram, *uptime;						//State

    // MainMenu:
    configuration = MainMenu("Configuration");
    network = MainMenu("Network");
    state = MainMenu("State");


    // SubMenu Configuration:
    mintemp = SubMenu(configuration, "Min Temp");
    maxtemp = SubMenu(configuration, "Max Temp");
    //start = SubMenu(configuration, "Start Time");
    duration = SubMenu(configuration, "Duration");

    // SubMenu Network:
    ipchoose = SubMenu(network, "IP-Address: Dyn/Static");
    ip = SubMenu(network, "IP-Address");
    //netaddress = SubMenu(network, "Net-Address");
    //gateway = SubMenu(network, "Gateway");
    //dns = SubMenu(network, "DNS-Server");

    // SubMenu State:
    temperature = SubMenu(state, "Temperature");
    cpu = SubMenu(state, "CPU");
    ram = SubMenu(state, "RAM");
    uptime = SubMenu(state, "Uptime");


    // Funktionen werden den SubMenu's zugeordnet:
    RegisterMenu(mintemp, Screen_MinTemp);
    RegisterMenu(maxtemp, Screen_MaxTemp);
    RegisterMenu(duration, Screen_Duration);
    RegisterMenu(ip, Screen_ip);

    /*
    DefaultRow2("<      OK       >");

    //DisplayConfig(ROWS,COLS);
    */
	usleep(1000);
    RegisterNumberOfButtons(4);
    RegisterMenuButtons(0, 1, 3);

    //Endlosschleife des Users beginnt hier:
    while(1)
    {
        run(); // Führt einen Menuschritt aus!
usleep(30000);

        //Weiter Code des Users. Steuerung von gpio's usw.
    }
    return 0;
}

// User Funktionen:

// User Funktion zum einstellen der minimalen Temperatur
void Screen_MinTemp()
{
    char buffer[16];
    int exit = 0;
    static int a = 22;
    while(!exit)
    {
        printf("Temperature: %i\n", a);
        printf("-      OK      +\n");

        sprintf(buffer, "Temperature: %i", a);
        DisplayRow1(buffer);
        DisplayRow2("-      OK      +");

        switch(event())
        {
        case 0:
            a--;
            break;
        case 1:
            exit = 1;
            break;
        case 3:
            a++;
            break;
        default:
            break;
        }
    }
}

// User Funktion zum einstellen der maximalen Temperatur
void Screen_MaxTemp()
{
    char buffer[16];
    int exit = 0;
    static int a = 22;
    while(!exit)
    {
        printf("Temperature: %i\n", a);
        printf("-      OK      +\n");

        sprintf(buffer, "Temperature: %i", a);
        DisplayRow1(buffer);
        DisplayRow2("-      OK      +");

        switch(event())
        {
        case 0:
            a--;
            break;
        case 1:
            exit = 1;
            break;
        case 3:
            a++;
            break;
        default:
            break;
        }
    }
}

// User Funktion zum einstellen der Dauer
void Screen_Duration()
{
    char buffer[16];
    int exit = 0;
    static int a = 22;
    while(!exit)
    {
        printf("Time: %i\n", a);
        printf("-      OK      +\n");

        sprintf(buffer, "Time: %i", a);
        DisplayRow1(buffer);
        DisplayRow2("-      OK      +");

        switch(event())
        {
        case 0:
            a--;
            break;
        case 1:
            exit = 1;
            break;
        case 3:
            a++;
            break;
        default:
            break;
        }
    }
}


void Screen_ip()     // Seit dem Hinzufuegen der Buttons fehlerhaft
{
    int i = 0;
    char buffer[16];
    static char ip[16]= {'0','0','0','.','0','0','0','.','0','0','0','.','0','0','0', 0};
    while(i < 15)
    {
        printf("%s  \n", ip);
        printf("-      OK      +\n\n");
        sprintf(buffer, "%s  \n", ip);
        DisplayRow1(buffer);
        DisplayRow2("-      OK      +");
        switch(event())
        {
        case 0:
            if(ip[i] > '0') ip[i]--;
            break;
        case 1:
            if(i == 2 || i == 6 || i == 10) i++;  // Punkte überspringen
            i++;
            break;
        case 3:
            if(ip[i] < '9') ip[i]++;
            break;
        default:
            break;
        }
    }
}
