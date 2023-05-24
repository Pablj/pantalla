
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"

const char* saludo();
void lcd_string(const char *s);
void lcd_set_cursor(int line, int position);
void lcd_init(int pin_sda,int pin_scl);
void lcd_clear(void);
