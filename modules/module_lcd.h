////////////////////////////////////////////////////////////////////////////////
//connection on the Portexpander Port 0
#define LCD_EN			0x04
#define LCD_RS			0x01
#define LCD_RW			0x02
////////////////////////////////////////////////////////////////////////////////
// LCD Timings
#define LCD_BOOTUP_MS           15000
#define LCD_ENABLE_US           20
#define LCD_WRITEDATA_US        46
#define LCD_COMMAND_US          42

#define LCD_SOFT_RESET_MS       5000
#define LCD_SET_8BITMODE_MS     5000

#define LCD_CLEAR_DISPLAY_MS    2000
#define LCD_CURSOR_HOME_MS      2000

////////////////////////////////////////////////////////////////////////////////
//definitions of the lines of an 4x20 Display

#define LCD_DDADR_LINE1         0x00
#define LCD_DDADR_LINE2         0x40
#define LCD_DDADR_LINE3         0x14
#define LCD_DDADR_LINE4         0x54

////////////////////////////////////////////////////////////////////////////////
// LCD instructions and arguments
// to use with lcd_command

// Clear Display -------------- 0b00000001
#define LCD_CLEAR_DISPLAY       0x01

// Cursor Home ---------------- 0b0000001x
#define LCD_CURSOR_HOME         0x02

// Set Entry Mode ------------- 0b000001xx
#define LCD_SET_ENTRY           0x04

#define LCD_ENTRY_DECREASE      0x00
#define LCD_ENTRY_INCREASE      0x02
#define LCD_ENTRY_NOSHIFT       0x00
#define LCD_ENTRY_SHIFT         0x01

// Set Display ---------------- 0b00001xxx
#define LCD_SET_DISPLAY         0x08

#define LCD_DISPLAY_OFF         0x00
#define LCD_DISPLAY_ON          0x04
#define LCD_CURSOR_OFF          0x00
#define LCD_CURSOR_ON           0x02
#define LCD_BLINKING_OFF        0x00
#define LCD_BLINKING_ON         0x01

// Set Shift ------------------ 0b0001xxxx
#define LCD_SET_SHIFT           0x10

#define LCD_CURSOR_MOVE         0x00
#define LCD_DISPLAY_SHIFT       0x08
#define LCD_SHIFT_LEFT          0x00
#define LCD_SHIFT_RIGHT         0x04

// Set Function --------------- 0b001xxxxx
#define LCD_SET_FUNCTION        0x20

#define LCD_FUNCTION_4BIT       0x00
#define LCD_FUNCTION_8BIT       0x10
#define LCD_FUNCTION_1LINE      0x00
#define LCD_FUNCTION_2LINE      0x08
#define LCD_FUNCTION_5X7        0x00
#define LCD_FUNCTION_5X10       0x04

#define LCD_SOFT_RESET          0x30

// Set DD RAM Address --------- 0b1xxxxxxx  (Display Data RAM)
#define LCD_SET_DDADR           0x80



/**
* @class gnublin_module_lcd
* @~english
* @brief Class for accessing GNUBLIN Module-LCD 4x20
*
* The GNUBLIN Module-LCD_4x20 can be easily controlled with the gnublin_lcd API. The Display uses the I2C-Bus.
* @~german 
* @brief Klasse für den zugriff auf das GNUBLIN Module-LCD 4x20
*
* Das GNUBLIN Module-LCD_4x20 lässt sich mit Hilfe der gnublin_lcd API ganz einfach ansteuern. Das Display nutzt die I2C-Schnittstelle.  
*/ 
class gnublin_module_lcd {
		bool error_flag;
		gnublin_module_pca9555 pca;
		std::string ErrorMessage;
		const char *version;
public:
		gnublin_module_lcd();
		const char *getErrorMessage();
		bool fail();
		void setAddress(int Address);
		void setDevicefile(std::string filename);
		int out(unsigned char rsrw, unsigned char data );
		int sendData(unsigned char data);
		int command(unsigned char data);
		int clear();
		int home();
		int setdisplay(int cursor, int blink);
		int setcursor(unsigned char x, unsigned char y);
		int string(const char *data);
		int init();
		
};
