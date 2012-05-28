echo "insmod module"
rmmod simple_char
insmod simple_char.ko

rm -f test_char
DEV_NO=`cat /proc/devices|grep demo_cdev |awk '{print $1;}' |head -1`
mknod test_char c $DEV_NO 0
chown zenki:zenki test_char

echo "mkdev test_char major =" $DEV_NO
