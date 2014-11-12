#!/usr/bin/gnuplot

date = system("date +%F-%H-%M-%S")
filename = "benchmark_array-".date.".png"

set output filename

set terminal png

set title "ccr runtime for multiple array sizes (200 runs)"

set xlabel "array size"
set ylabel "time (s)"

set key on outside right
set logscale y
set xrange [3500:17000]
set bmargin 4
set grid

set style line 1 lc rgb "#6B8E23"  lt 1 lw 1.5 ps 1 pi -1  # --- green
set style line 2 lc rgb "#104E8B"  lt 1 lw 1.5 ps 1 pi -1  # --- blue
set style line 3 lc rgb "#CD3333"  lt 1 lw 1.5 ps 1 pi -1  # --- red
set style line 4 lc rgb "#000000"  lt 1 lw 1.5 ps 1 pi -1  # --- black

plot "array.txt" index 0 using 1:2:xtic(1) ls 1 pt 7  title "iterative " w lp,\
     ""			 index 1 using 1:2:xtic(1) ls 2 pt 7  title "recursive" w lp,\
     ""			 index 2 using 1:2:xtic(1) ls 3 pt 7  title "parallel" w lp,\
     ""			 index 3 using 1:2:xtic(1) ls 4 pt 7  title "bruteforce" w lp