#!/usr/bin/sh
index="0"
while true
do
	./rb.out
	index=$((index+1))
	if [ ${index} -eq 10000 ];then
		break
	fi
done
