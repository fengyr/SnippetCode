#!/usr/bin/env bash

echo "remove old so"
rm -f libs/libapp.so

# 1. 拷贝所需的app_engine头文件，排除interface目录、runtime.h以及options.h
echo "========== update include =========="
rsync -av /home/zenki/snippet_code/c/libraries/app_engine/include . \
    --exclude=interface/ \
    --exclude=runtime.h \
    --exclude=options.h \
    --exclude=*.swp

# 2. 最终更新的目标库文件
echo "========== update libapp.so =========="
cp /home/zenki/snippet_code/c/libraries/app_engine/libapp.so libs/

# 以下为应用相关的第三方库和头文件
echo "========== update other so=========="
# echo "copy libvcamgetpic.so"
# cp /home/zenki/ruizhan/ColorPick/setpicV3/build/libvcamgetpic.so libs/
# echo "copy libio.so"
# cp /home/zenki/ruizhan/Tools/Extend/libDIO/libio.so libs/
# echo "copy libpci_dask.so"
# cp /home/zenki/ruizhan/Tools/Extend/libDIO/libs/libpci_dask.so libs/
# echo "copy DIO7230.h"
# cp /home/zenki/ruizhan/Tools/Extend/libDIO/DIO7230.h ./include/interface/
# echo "copy getpic.h"
# cp /home/zenki/ruizhan/ColorPick/setpicV3/include/vcamgetpic/getpic.h ./include/interface/
# echo "copy typedef.h"
# cp /home/zenki/ruizhan/ColorPick/setpicV3/include/typedef.h ./include/interface/
