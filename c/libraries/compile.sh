#!/usr/bin/env bash

SRC=`pwd`

echo "========= compile dev_serial... ========="
cd $SRC/dev_serial/
make clean
make
cd $SRC

echo "========= compile libmodbus... ========="
echo $SRC
cd $SRC/libmodbus
make clean
rm -f $SRC/libmodbus/src/.libs/libmodbus.a
./configure
make
cd $SRC/libmodbus/src/.libs 
ar rcvs libmodbus.a modbus-data.o modbus.o modbus-rtu.o modbus-tcp.o
cp libmodbus.a $SRC/app_engine/libs/
cd $SRC
