OBJ := adc gpio_output ledblink module_adc module_lcd_4x20 module_relay module_temperature spi gpio_input i2c module_lcd_2x16 module_pca9555 module_step printer printer_temp
CLEANOBJ := $(OBJ:%=clean-%)
CPP := arm-linux-gnueabi-g++


.PHONY: $(OBJ)

all: $(OBJ)
$(OBJ): gnublin.cpp gnublin.h
	$(CPP) -Wall -o $@ $@.cpp gnublin.cpp

clean: $(CLEANOBJ)
$(CLEANOBJ):
	rm -f $(@:clean-%=%)
