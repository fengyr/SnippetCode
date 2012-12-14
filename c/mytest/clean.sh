#!/bin/bash

DIRS=`ls`

for dir in $DIRS; do
    if [[ -d $dir && -f $dir/Makefile ]]; then
        cd $dir && make clean
        cd ..
    fi
done
