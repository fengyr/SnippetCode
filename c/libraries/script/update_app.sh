#!/usr/bin/env bash

echo "remove other so"
rm -f libs/libapp.so
rm -f libs/libio.so
rm -f libs/libpci_dask.so

# update app_engine src
echo "========== update include =========="
rsync -av /home/zenki/snippet_code/c/libraries/app_engine/include . \
    --exclude=interface/ \
    --exclude=runtime.h \
    --exclude=options.h \
    --exclude=*.swp

# update libapp.so
echo "========== update libapp.so =========="
cp /home/zenki/snippet_code/c/libraries/app_engine/libapp.so libs/

# update other so && .h
echo "========== update other so=========="
echo "copy libvcamgetpic.so"
cp /home/zenki/ruizhan/ColorPick/setpicV3/build/libvcamgetpic.so libs/
echo "copy libio.so"
cp /home/zenki/ruizhan/Tools/Extend/libDIO/libio.so libs/
echo "copy libpci_dask.so"
cp /home/zenki/ruizhan/Tools/Extend/libDIO/libs/libpci_dask.so libs/
echo "copy DIO7230.h"
cp /home/zenki/ruizhan/Tools/Extend/libDIO/DIO7230.h ./include/interface/
echo "copy getpic.h"
cp /home/zenki/ruizhan/ColorPick/setpicV3/include/vcamgetpic/getpic.h ./include/interface/
echo "copy typedef.h"
cp /home/zenki/ruizhan/ColorPick/setpicV3/include/typedef.h ./include/interface/
