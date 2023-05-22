
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "LCD_16_02.h"

#define POSICION_INICIAL_X 0
#define POSICION_INICIAL_Y 0

const int PIN_SDA= 12;
const int PIN_SCL= 13;

volatile bool button_Y_pressed = false;
volatile bool button_A_pressed = false;
volatile bool button_X_pressed = false;
volatile bool button_B_pressed = false;


static char *menu[][4] = {
    {"Estado", "Temperatura", "Motor", "Bateria"},
    {"Acceso Manual", "Motor","",""},
    {"Ajustes", "", "", ""}
};

void init(){
    i2c_init_local(PIN_SDA,PIN_SCL);
    botton_init();
}




int main() {

    init();   
    int posicion_actual_x=POSICION_INICIAL_X;
    int posicion_actual_y=POSICION_INICIAL_Y;	
   
    
    while (1) {
    
      	mostrarPantalla(posicion_actual_x,posicion_actual_y, menu[posicion_actual_y,posicion_actual_x] );
        if (button_Y_pressed) {
            if(posicion_actual_y > 0 ){
            	posicion_actual_y--;
            	lcd_clear();
            }
            
            
            button_Y_pressed = false;  // Reiniciar variable
        }
        if (button_A_pressed) {
            if(posicion_actual_y < sizeof(menu) / sizeof(menu[0]) -1 ){
            //variable aux
            	posicion_actual_y++;
      		    lcd_clear();
            }
            
            button_A_pressed = false;  // Reiniciar variable
        }
        if (button_X_pressed) {
            // Hacer algo cuando se pulsa el botón X
            button_X_pressed = false;  // Reiniciar variable
        }
        if (button_B_pressed) {
            // Hacer algo cuando se pulsa el botón X
            button_B_pressed = false;  // Reiniciar variable
        }
      
    }

    return 0;


}

