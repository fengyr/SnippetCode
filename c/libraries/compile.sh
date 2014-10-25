#!/usr/bin/env bash

SRC=`pwd`

echo "========= compile dev_serial... ========="
cd $SRC/dev_serial/
make clean
make
cd $SRC

echo "========= compile libmodbus... ========="
cd $SRC/libmodbus
./autogen.sh
make clean
rm -f $SRC/libmodbus/src/.libs/libmodbus.a
./configure
make
cd $SRC/libmodbus/src/.libs 
ar rcvs libmodbus.a modbus-data.o modbus.o modbus-rtu.o modbus-tcp.o
cp libmodbus.a $SRC/app_engine/libs/
cd $SRC

echo "========= update simpleini... ========="
cp $SRC/simpleini/ConvertUTF.h $SRC/app_engine/include/core/utils/
cp $SRC/simpleini/ConvertUTF.c $SRC/app_engine/src/core/utils/
cp $SRC/simpleini/SimpleIni.h $SRC/app_engine/include/core/utils/
