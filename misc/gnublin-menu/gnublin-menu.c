#include "gnublin-menu.h"


int event()
{
    int button;

    int val;
    int i=0;
    while (1)
    {
	usleep(10000);
        val = selectADC(0,fd);
        if(val < 800) return button_prev;
        val = selectADC(1,fd);
        if(val < 800) return button_ok;
        val = selectADC(3,fd);
        if(val < 800) return button_next;
    }
}

void run()
{
    static int i=0;
    if(i == 0)
    {
        zeiger = root->child[0];
        i++;
    }

    print(zeiger);

    int ret = event();
    if(ret == button_prev) prev();
    else if(ret == button_ok) ok();
    else if(ret == button_next) next();
    else if(ret < 0 ) printf("Kein gültiger Button.\n1: prev/back\n2: ok\n3: next\n");
}


void prev()
{
    int i=0;
    if(zeiger->menu_type == 2)
    {
        zeiger = zeiger->mother;		// if sub go to mother
    }
    else
    {
        if(zeiger->pos != 0)  				//if is not at begin go to left
        {
            i = zeiger->pos - 1;
            zeiger = zeiger->mother->child[i];  // check this 
        }
        else
        {
            i=0;
            while(zeiger->mother->child[i+1] != NULL)	   
            {
                zeiger = zeiger->mother->child[i];				
                i++;
            }
            zeiger = zeiger->mother->child[i];			
        }
    }

}

void ok()
{
    if(zeiger->fp == NULL)    
    {
        if(zeiger->child[0] != NULL)
        {
            zeiger = zeiger->child[0];
        }
    }
    else
    {
        (*zeiger->fp)();   
    }
}


void next()
{
    int i=0;
    if(zeiger->mother->child[zeiger->pos + 1] != NULL )
    {
        zeiger = zeiger->mother->child[zeiger->pos + 1];
    }
    else
    {
        zeiger = zeiger->mother->child[0];		
    }

}

void RegisterMenu (struct menu *ptr, void (*fp)(void))
{
    ptr->fp = fp;
}

struct menu* MainMenu(char* name)
{
    static int new = 0;
    int i=0;
    int found=0;
    if ( new == 0 )
    {
        if((root = malloc(sizeof(struct menu))) == NULL)     
        {
            printf("Nicht genügend Speicherplatz für root!\n");
        }
        root->menu_type = 0;
        root->pos = 0;
        root->fp = NULL;
        strcpy(root->top, "root");
        root->mother = root;

        while(i<16)
        {
            root->child[i] = NULL;
            i++;
        }
        new++;
    }
    i=0;
    while(!found)
    {
        if(root->child[i] == NULL )
        {
            if((root->child[i] = malloc(sizeof(struct menu))) == NULL)                // Speicherplatzüberprüfung
            {
                printf("Nicht genügend Speicherplatz für MainMenu (root->child[%i])", i);
            }
            zeiger = root->child[i];  
            found = 1;      
        }
        if ( i > 16 )
        {
            printf("Zu wenig Speicherplatz für ein weiteres MainMenu (root->child[i])");
            found = 1;
        }
        i++;
    }
    zeiger->menu_type = 1;
    zeiger->pos = i - 1;  // write  zeiger->pos position from menu
    strcpy(zeiger->top, name);
    strcpy(zeiger->bottom, "<      OK      >");
    zeiger->fp = NULL;
    zeiger->mother = root;
    while(i<16)
    {
        zeiger->child[i] = NULL;
        i++;
    }
    i=0;
    return zeiger;
}

struct menu* SubMenu(struct menu *pointer, char* name)
{
    int i=0;
    int found=0;
    while(!found)
    {
        if(pointer->child[i] == NULL )
        {
            if((pointer->child[i] = malloc(sizeof(struct menu))) == NULL)                
            {
                printf("not enough memory (root->child[%i])", i);
            }

            zeiger = pointer->child[i];       
            found = 1;                     
        }
        if ( i > 16 )
        {
            printf("not enough memory (root->child[i])");
            found = 1;
        }
        i++;
    }
    zeiger->menu_type = 2;
    zeiger->pos = i-1;
    strcpy(zeiger->top, name);
    strcpy(zeiger->bottom, "back   OK      >");
    zeiger->fp = NULL;
    zeiger->mother = pointer;
    while(i<16)
    {
        zeiger->child[i] = NULL;
        i++;
    }
    i=0;
    return zeiger;
}

int DisplayRow1(char *text)
{
    char * const argv_cryptsetup[] =
    {
        DOGM,
        "-n",
        "-w",
        text,
        NULL
    };

    setuid(0);
    setgid(0);
    if ( fork() == 0 )
        execvp(DOGM, argv_cryptsetup);
    else
        wait (NULL);
    return 0;
}

int DisplayRow2( char *text)
{
    char * const argv_cryptsetup[] =
    {
        DOGM,
        "-o",
        "192",
        "-w",
        text,
        NULL
    };

    setuid(0);
    setgid(0);
    if ( fork() == 0 )
        execvp(DOGM, argv_cryptsetup);
    else
        wait (NULL);

    return 0;
}

void print(struct menu *print_ptr)
{

    printf("%s\n%s\n", print_ptr->top, print_ptr->bottom);

    DisplayRow1(print_ptr->top);
    DisplayRow2(print_ptr->bottom);
}

void RegisterNumberOfButtons (int anzahl)
{
    button_anzahl = anzahl;
}

void RegisterMenuButtons (int prev, int ok, int next)
{
    button_prev = prev;
    button_ok = ok;
    button_next = next;
}

int selectADC( int channel, int fd )
{
    if ( channel < 0 || channel > 3 )
    {
        return EXIT_FAILURE;
    }

    int fdi;
    int ret;
    char sChannel[2] = { '0' + channel , '\0' };

    int val;
    char sVal[6]; /* { '0' , 'x' , '3 ' , 'F' , 'F' , '\0' } */


    fdi = open( "/dev/lpc313x_adc" , O_RDWR );
    ret = write( fdi , sChannel , sizeof(sChannel) );

    ret = close ( fdi );
    fdi = open( "/dev/lpc313x_adc" , O_RDWR );
    
    ret = read( fdi , sVal , sizeof(sVal) );

    ret = close( fdi );

    sscanf( sVal , "0x%x" , &val );

    return val;
}

int getADC(void)
{
    int fd;
    int ret;
    int val;
    char sVal[6]; /* { '0' , 'x' , '3 ' , 'F' , 'F' , '\0' } */


    fd = open( "/dev/lpc313x_adc" , O_RDONLY );
    if ( fd < 0 )
    {
        return -1;
    }
    ret = read( fd , sVal , sizeof(sVal) );
    if ( ret != sizeof(sVal) )
    {
        close( fd );
        return -2;
    }
    ret = close( fd );
    if ( ret != EXIT_SUCCESS )
    {
        return -3;
    }

    ret = sscanf( sVal , "0x%x" , &val );
    if ( ret != 1 )
    {
        return -4;
    }

    return val;
}
