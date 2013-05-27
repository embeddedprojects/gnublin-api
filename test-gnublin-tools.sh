#!/bin/bash          
echo "##########################" > test.log
echo "# Test Log Gnublin-tools #" >> test.log
echo "##########################" >> test.log
date >> test.log

#Test relay module
echo "START testing relay module" >> test.log
for i in {1..8}
do
   gnublin-relay -o 1 -p $i >> test.log
done

for i in {1..8}
do
   gnublin-relay -o 0 -p $i >> test.log
done
echo "STOP testing relay module" >> test.log

#Test dogm module
echo "START testing dogm module" >> test.log
gnublin-dogm -i >> test.log
gnublin-dogm -l 1 -w "Line 1" >> test.log
sleep 0.5
gnublin-dogm -l 2 -w "Line 2" >> test.log
sleep 0.5
gnublin-dogm -d >> test.log
gnublin-dogm -o 5 -w "test" >> test.log
sleep 0.5
gnublin-dogm -o 25 -w "test" >> test.log
sleep 0.5
gnublin-dogm -n >> test.log
echo "STOP testing dogm module" >> test.log

#Test lm75 module
echo "START testing lm75 module" >> test.log
gnublin-lm75 >> test.log
gnublin-lm75 -j >> test.log
gnublin-lm75 -b >> test.log
echo "STOP testing lm75 module" >> test.log

#Test step module
echo "START testing step module" >> test.log
gnublin-step -a76 -p 6400
sleep 2
echo "STOP testing step module" >> test.log


#Test LCD module
echo "START testing lcd module" >> test.log

gnublin-lcd -a 0x21 -i >> test.log
gnublin-lcd -a 0x21 -d >> test.log

for i in {1..4}
do
   gnublin-lcd -a 0x21 -l $i -w "Line $i" >> test.log
done

for i in {1..4}
do
   gnublin-lcd -a 0x21 -l $i -o 9 -w "bla" >> test.log
done

echo "STOP testing lcd module" >> test.log

less test.log
