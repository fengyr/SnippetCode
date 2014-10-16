#!/usr/bin/env bash

PWD=`pwd`
SRC="/home/zenki/snippet_code"
FOLD_CODE="c/libraries/app_engine"
FOLD_SCRIPT="$FOLD_CODE/script"

INIT=$1

function init()
{
    mkdir $PWD/include
    mkdir $PWD/src
    mkdir $PWD/libs

    update 

    echo "========== Add Frame Code =========="
    cp -rf $SRC/$FOLD_SCRIPT/include .
    cp -rf $SRC/$FOLD_SCRIPT/src .
    cp -rf $SRC/$FOLD_SCRIPT/Makefile .
}

function update()
{
    echo "remove old so"
    rm -f $PWD/libs/libapp.so

    # 1. 拷贝所需的app_engine头文件，排除interface目录、runtime.h以及options.h
    echo "========== update include =========="
    rsync -av $SRC/$FOLD_CODE/include . \
        --exclude=interface/* \
        --exclude=runtime.h \
        --exclude=options.h \
        --exclude=*.swp

    # 2. 最终更新的目标库文件
    echo "========== update libapp.so =========="
    cp $SRC/$FOLD_CODE/libapp.so libs/

    # 以下为应用相关的第三方库和头文件
    echo "========== update other so=========="
    # echo "copy libio.so"
    # cp /home/zenki/ruizhan/Tools/Extend/libDIO/libio.so libs/
}

if [[ $INIT == "init" ]]; then
    init
else
    update
fi
