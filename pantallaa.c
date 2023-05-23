
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"

/* Example code to drive a 16x2 LCD panel via a I2C bridge chip (e.g. PCF8574)

   NOTE: The panel must be capable of being driven at 3.3v NOT 5v. The Pico
   GPIO (and therefor I2C) cannot be used at 5v.

   You will need to use a level shifter on the I2C lines if you want to run the
   board at 5v.

   Connections on Raspberry Pi Pico board, other boards may vary.

   GPIO 4 (pin 12)-> SDA on LCD bridge board
   GPIO 5 (pin 13)-> SCL on LCD bridge board
   3.3v (pin 36) -> VCC on LCD bridge board
   GND (pin 38)  -> GND on LCD bridge board
*/
// commands
const int LCD_CLEARDISPLAY = 0x01;
const int LCD_RETURNHOME = 0x02;
const int LCD_ENTRYMODESET = 0x04;
const int LCD_DISPLAYCONTROL = 0x08;
const int LCD_CURSORSHIFT = 0x10;
const int LCD_FUNCTIONSET = 0x20;
const int LCD_SETCGRAMADDR = 0x40;
const int LCD_SETDDRAMADDR = 0x80;

// flags for display entry mode
const int LCD_ENTRYSHIFTINCREMENT = 0x01;
const int LCD_ENTRYLEFT = 0x02;

// flags for display and cursor control
const int LCD_BLINKON = 0x01;
const int LCD_CURSORON = 0x02;
const int LCD_DISPLAYON = 0x04;

// flags for display and cursor shift
const int LCD_MOVERIGHT = 0x04;
const int LCD_DISPLAYMOVE = 0x08;

// flags for function set
const int LCD_5x10DOTS = 0x04;
const int LCD_2LINE = 0x08;
const int LCD_8BITMODE = 0x10;

// flag for backlight control
const int LCD_BACKLIGHT = 0x08;

const int LCD_ENABLE_BIT = 0x04;

// By default these LCD display drivers are on bus address 0x27
static int addr = 0x27;
#define POSICION_INICIAL_X 0
#define POSICION_INICIAL_Y 0
// Modes for lcd_send_byte
#define LCD_CHARACTER  1
#define LCD_COMMAND    0

#define MAX_LINES      2
#define MAX_CHARS      16

#define BUTTON_Y 18
#define BUTTON_A 16
#define BUTTON_X 15
#define BUTTON_B 19


const char button_Y_pressed = 'Y';
const char button_A_pressed = 'A';
const char button_X_pressed = 'X';
const char button_B_pressed = 'B';

volatile char button_pressed;
static char *menu[][3][4] = {
    {
        {"Menu", "Temperatura", "Motor", "Bateria"},
        {"Menu", "Motor", "", ""},
        {"Menu", "", "", ""}
    },
    {
        {"Estado", "T:002", "C:004", "B:4% I:4[A]"},
        {"Acceso Manual", "si", "", ""},
        {"Ajustes", "", "", ""}
    }
};
char opciones[2][16];
/* Quick helper function for single byte transfers */
void i2c_write_byte(uint8_t val) {
#ifdef i2c_default
    i2c_write_blocking(i2c_default, addr, &val, 1, false);
#endif
}

void lcd_toggle_enable(uint8_t val) {
    // Toggle enable pin on LCD display
    // We cannot do this too quickly or things don't work
#define DELAY_US 600
    sleep_us(DELAY_US);
    i2c_write_byte(val | LCD_ENABLE_BIT);
    sleep_us(DELAY_US);
    i2c_write_byte(val & ~LCD_ENABLE_BIT);
    sleep_us(DELAY_US);
}

// The display is sent a byte as two separate nibble transfers
void lcd_send_byte(uint8_t val, int mode) {
    uint8_t high = mode | (val & 0xF0) | LCD_BACKLIGHT;
    uint8_t low = mode | ((val << 4) & 0xF0) | LCD_BACKLIGHT;

    i2c_write_byte(high);
    lcd_toggle_enable(high);
    i2c_write_byte(low);
    lcd_toggle_enable(low);
}

void lcd_clear(void) {
    lcd_send_byte(LCD_CLEARDISPLAY, LCD_COMMAND);
}

// go to location on LCD
void lcd_set_cursor(int line, int position) {
    int val = (line == 0) ? 0x80 + position : 0xC0 + position;
    lcd_send_byte(val, LCD_COMMAND);
}

static void inline lcd_char(char val) {
    lcd_send_byte(val, LCD_CHARACTER);
}

void lcd_string(const char *s) {
    while (*s) {
        lcd_char(*s++);
    }
}

void lcd_init() {
    lcd_send_byte(0x03, LCD_COMMAND);
    lcd_send_byte(0x03, LCD_COMMAND);
    lcd_send_byte(0x03, LCD_COMMAND);
    lcd_send_byte(0x02, LCD_COMMAND);

    lcd_send_byte(LCD_ENTRYMODESET | LCD_ENTRYLEFT, LCD_COMMAND);
    lcd_send_byte(LCD_FUNCTIONSET | LCD_2LINE, LCD_COMMAND);
    lcd_send_byte(LCD_DISPLAYCONTROL | LCD_DISPLAYON, LCD_COMMAND);
    lcd_clear();
}

void button_isr(uint gpio, uint32_t events) {
    if (gpio == BUTTON_Y) {
        if (gpio_get(BUTTON_Y)) {
            // El botón Y ha sido presionado
            button_pressed = button_Y_pressed; 
            
        } else {
            // El botón Y ha sido liberado
        }
    } else if (gpio == BUTTON_X) {
        if (gpio_get(BUTTON_X)) {
            // El botón X ha sido presionado
           button_pressed = button_X_pressed; 
           
        } else {
            // El botón X ha sido liberado
        }
    } else if (gpio == BUTTON_A) {
        if (gpio_get(BUTTON_A)) {
            button_pressed = button_A_pressed;
            
            // El botón A ha sido presionado
        } else {
            // El botón A ha sido liberado
        }
    } else if (gpio == BUTTON_B) {
        
            button_pressed = button_B_pressed; 
            
            // El botón B ha sido presionado
        
    }
}


void botton_init(){
    gpio_init(BUTTON_Y );
    gpio_init(BUTTON_X );
    gpio_init(BUTTON_B );
    gpio_init(BUTTON_A );
     
    gpio_set_dir(BUTTON_Y , GPIO_IN);
    gpio_set_dir(BUTTON_X , GPIO_IN);
    gpio_set_dir(BUTTON_B , GPIO_IN);
    gpio_set_dir(BUTTON_A , GPIO_IN);
    
    gpio_pull_down(BUTTON_Y);
    gpio_pull_down(BUTTON_X);
    gpio_pull_down(BUTTON_B);
    gpio_pull_down(BUTTON_A);
    
    gpio_set_irq_enabled_with_callback(BUTTON_Y, GPIO_IRQ_EDGE_RISE, true, &button_isr);
    gpio_set_irq_enabled_with_callback(BUTTON_X, GPIO_IRQ_EDGE_RISE, true, &button_isr);
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_RISE, true, &button_isr);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_RISE, true, &button_isr);
    
}
void i2c_init_local(){
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(12, GPIO_FUNC_I2C);
    gpio_set_function(13, GPIO_FUNC_I2C);
    gpio_pull_up(12);
    gpio_pull_up(13);
    // Make the I2C pins available to picotool
    bi_decl(bi_2pins_with_func(12, 13, GPIO_FUNC_I2C));
}
void init(){
    i2c_init_local();
    lcd_init();
    botton_init();
}


void mostrarPantalla(int posicion_x,int posicion_y){

     for(int i=0;i<2;i++){
    sprintf(opciones[i], "%s", menu[i][posicion_y][posicion_x]);
    lcd_set_cursor(i,0);
    lcd_string(opciones[i]);
    }
    
    
}


int main() {

    init();   
    int posicion_actual_x=POSICION_INICIAL_X;
    int posicion_actual_y=POSICION_INICIAL_Y;	
    const int columnas=sizeof(menu[0]) / sizeof(int) - 1;
    const int filas=sizeof(menu) / sizeof(menu[0]) -1 ;
    
    while (1) {
    
      	mostrarPantalla(posicion_actual_x, posicion_actual_y);

	switch (button_pressed) {
	    case 'Y':
		if (posicion_actual_y > 0) {
		    posicion_actual_y--;
		    lcd_clear();
		    button_pressed='N';
		    
		}
		break;

	    case 'A':
		if (posicion_actual_y < filas) {
		    posicion_actual_y++;
		    lcd_clear();
		    button_pressed='N';
		}
		break;

	    case 'X':
		if (posicion_actual_x > 0) {
		    posicion_actual_x--;
		    lcd_clear();
		    button_pressed='N';
		}
		break;

	    case 'B':
		if (posicion_actual_x < columnas) {
		    if (menu[posicion_actual_y][posicion_actual_x + 1] != "") {
		        posicion_actual_x++;
		        lcd_clear();
		        button_pressed='N';
		    }
		}
		break;

	    default:
		// Opción por defecto si no se cumple ninguna de las condiciones anteriores
		break;
	}

	      
	    }

	    return 0;


}
