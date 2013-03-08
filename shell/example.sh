#!/usr/bin/env bash

####################{}进行扩展
#FILE=/etc/fstab
#
#{
#	read line1
#	read line2
#}<$FILE

#echo $line1
#echo $line2

####################()运行子shell
#(ls;top;)

####################let 进行运算操作
#a=100
#echo $a
#let "a += 1"
#echo $a

###################${//}进行字符替换
#a="i am a student"
#echo $a
#b=${a/am/is}
#echo $b

###################$0, $#代表程序运行名称参数等
#echo $0
echo $1
#echo $#
#echo $*
#echo $@

i=0
ls | while read line
do
    echo $line
    let i++
done  

echo $i

echo "hello"
