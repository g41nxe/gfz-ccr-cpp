#!/bin/bash

rm -f test.txt
touch test.txt

for type in 0 1 2
do	
	OUTUT=""
	for size in 1 2.5 5 10
	do
		/usr/bin/time -f%U -otime $1 $type $size
		OUTPUT+=`cat time`
		OUTPUT+="\t"
	done
	echo -e $OUTPUT >> test.txt
done

rm -f time

