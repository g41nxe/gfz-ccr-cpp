#!/bin/bash

rm -f $s.txt
touch $2.txt

echo -e "#rounds\t#1024\t#2048\t#4096" >> $2.txt
for rounds in 50 100 200 400
do
	OUTPUT="$rounds\t"	
	
	for size in 10 11 12
	do
		/usr/bin/time -f%U -otime $1 $size $rounds
		OUTPUT+=`cat time`
		OUTPUT+="\t"
	done
	
	echo -e $OUTPUT >> $2.txt
done

rm -f time

