add_executable(pantalla
        pantalla.c
        LCD_16_02.c
        LCD_16_02.h
        )

# pull in common dependencies
target_link_libraries(pantalla pico_stdlib  hardware_gpio hardware_i2c)  






# create map/bin/hex file etc.
pico_add_extra_outputs(pantalla)

# add url via pico_set_program_url
example_auto_set_url(pantalla)
