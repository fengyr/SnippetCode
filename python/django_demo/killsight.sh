#!/bin/sh

while true;do
	HTIME=`date +%H%M`
	if [ $HTIME == "0000" ];then
		killall python
		killall vlc
		sleep 100
	fi
	sleep 3
done
