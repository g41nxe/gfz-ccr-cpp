#!/bin/bash

file=benchmark/bench_$(date +%Y%m%d%H%M%S).txt
rm -f $file
touch $file

for type in -1 0 1 2 3
do	
	OUTPUT=`$1 $type`
	echo -e $OUTPUT >> $file
done

