#!/bin/bash

rm -f $s.txt
touch $2.txt

echo -e "#rounds\t#512\t#1024\t#2048" >> $2.txt
for rounds in 10 50 100 
do
	OUTPUT="$rounds\t"	
	
	for size in 9 10 11
	do
		/usr/bin/time -f%U -otime $1 $size $rounds $3
		OUTPUT+=`cat time`
		OUTPUT+="\t"
	done
	
	echo -e $OUTPUT >> $2.txt
done

rm -f time

