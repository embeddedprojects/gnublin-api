#!/bin/bash

#Wrapper Script to build the GNUBLIN API

export build_time="$(date '+%D %H:%M')"

#################################################
## Building gnublin.h
#################################################

echo building gnublin.h

echo "//********************************************
//GNUBLIN API -- HEADER FILE
//build date: $build_time
//******************************************** \n" > gnublin.h

cat include/includes.h >> include_tmp.h
sed -i "/#endif/d" include_tmp.h
cat include_tmp.h >> gnublin.h
rm include_tmp.h

cat include/functions.h >> gnublin.h

cat drivers/gpio.h >> gnublin.h
cat drivers/i2c.h >> gnublin.h
cat drivers/spi.h >> gnublin.h
cat drivers/adc.h >> gnublin.h
cat drivers/serial.h >> gnublin.h
cat drivers/pwm.h >> gnublin.h

cat modules/module_dogm.h >> gnublin.h
cat modules/module_lm75.h >> gnublin.h
cat modules/module_adc.h >> gnublin.h
cat modules/module_pca9555.h >> gnublin.h
cat modules/module_relay.h >> gnublin.h
cat modules/module_step.h >> gnublin.h
cat modules/module_lcd.h >> gnublin.h
cat modules/module_dac.h >> gnublin.h

cat library/mail/base64.h >> gnublin.h
cat library/mail/CSmtp.h >> gnublin.h
cat library/csv/csv.h >> gnublin.h

sed -i "s/#include \"..\/include\/includes.h\"/\/\/***** NEW BLOCK *****/g" gnublin.h
sed -i "/^#include \"/d" gnublin.h
echo \#endif >> gnublin.h

#################################################
## Building gnublin.cpp
#################################################

echo building gnublin.cpp

echo "//********************************************
//GNUBLIN API -- MAIN FILE
//build date: $build_time
//******************************************** \n" > gnublin.cpp

cat include/functions.cpp >> gnublin.cpp

cat drivers/gpio.cpp >> gnublin.cpp
cat drivers/i2c.cpp >> gnublin.cpp
cat drivers/spi.cpp >> gnublin.cpp
cat drivers/adc.cpp >> gnublin.cpp
cat drivers/serial.cpp >> gnublin.cpp
cat drivers/pwm.cpp >> gnublin.cpp

cat modules/module_dogm.cpp >> gnublin.cpp
cat modules/module_lm75.cpp >> gnublin.cpp
cat modules/module_adc.cpp >> gnublin.cpp
cat modules/module_pca9555.cpp >> gnublin.cpp
cat modules/module_relay.cpp >> gnublin.cpp
cat modules/module_step.cpp >> gnublin.cpp
cat modules/module_lcd.cpp >> gnublin.cpp
cat modules/module_dac.cpp >> gnublin.cpp

cat library/mail/base64.cpp >> gnublin.cpp
cat library/mail/CSmtp.cpp >> gnublin.cpp
cat library/csv/csv.cpp >> gnublin.cpp

sed -i "/^#include/d" gnublin.cpp
#sed -i "/^#include \"/d" gnublin.cpp
sed -i "6i#include \"gnublin.h\"\n\n" gnublin.cpp

