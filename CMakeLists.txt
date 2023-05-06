add_executable(pantallaa
        pantallaa.c
        )

# pull in common dependencies
target_link_libraries(pantallaa pico_stdlib  hardware_gpio hardware_i2c)  






# create map/bin/hex file etc.
pico_add_extra_outputs(pantallaa)

# add url via pico_set_program_url
example_auto_set_url(pantallaa)
