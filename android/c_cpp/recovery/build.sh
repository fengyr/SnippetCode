#!/bin/bash

# Just make recovery build force
find | xargs touch

# Setup source code path
recovery_dir=`pwd`
cd ../e2fsprogs/; e2fsprogs_dir=`pwd`; cd - > /dev/null

framework_dir=
echo -n "Setup framework path:"
while [ -z "${framework_dir}" ]
do
	read framework_dir
	if [ -d ${framework_dir} ]; then
		break
	else
		echo "framework path is not exist!!!"
	fi
done > /dev/null

cd ${framework_dir} 

. build/envsetup.sh
choosecombo

export framework_dir
export e2fsprogs_dir
mmm ${e2fsprogs_dir} ${recovery_dir} && make -f ${recovery_dir}/Makefile recoveryimage
