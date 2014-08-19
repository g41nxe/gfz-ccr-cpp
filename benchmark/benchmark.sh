#!/bin/bash

rm -f $2.txt
touch $2.txt

echo -e "#rounds\t#512\t#1024\t#2048" >> $2.txt
for rounds in 100 500 1000
do
	OUTPUT="$rounds\t"	
	
	for size in 10 11 12
	do
		echo -e $1": "$rounds" rounds, size: 2^"$size
		/usr/bin/time -f%U -otime $1 $size $rounds $3
		OUTPUT+=`cat time`
		OUTPUT+="\t"
	done
	echo -e $OUTPUT >> $2.txt
done

rm -f time

