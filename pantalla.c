
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "LCD_16_02.h"


#define POSICION_INICIAL_X 0
#define POSICION_INICIAL_Y 0

// Pines del LCD
#define PIN_SDA 12
#define PIN_SCL 13



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


void mostrarPantalla(int posicion_x,int posicion_y){

     for(int i=0;i<2;i++){
    sprintf(opciones[i], "%s", menu[i][posicion_y][posicion_x]);
    lcd_set_cursor(i,0);
    lcd_string(opciones[i]);
    }
    
    
}
void init(){
	bi_decl(bi_2pins_with_func(PIN_SDA, PIN_SCL, GPIO_FUNC_I2C));
	lcd_init(PIN_SDA,PIN_SCL);
	
    	botton_init(); 
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
