#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"


	void lcd_clear();
	void i2c_init_local(int pin_sda, int pin_scl);
	void mostrarPantalla(int posicion_x,int posicion_y,char* palabra);

