#!/usr/bin/gnuplot

date = system("date +%F-%H-%M-%S")
filename = "benchmark_threads-".date.".png"

set output filename

set terminal png

set title "ccr benchmark for different thread counts (array size 8192)"

set xlabel "# cross correlations"
set ylabel "time (s)"

set key on outside right
#set logscale y
set xrange [200:1050]
set bmargin 4
set grid

set style line 1 lc rgb "#6B8E23"  lt 1 lw 1.5 ps 1 pi -1  # --- green
set style line 2 lc rgb "#104E8B"  lt 1 lw 1.5 ps 1 pi -1  # --- blue
set style line 3 lc rgb "#CD3333"  lt 1 lw 1.5 ps 1 pi -1  # --- red
set style line 4 lc rgb "#000000"  lt 1 lw 1.5 ps 1 pi -1  # --- black

plot "omp_4.txt"  using 1:2:xtic(1) ls 1 pt 7  title "4 threads " w lp,\
	 "omp_8.txt"  using 1:2:xtic(1) ls 2 pt 7  title "8 threads" w lp,\
	 "omp_16.txt" using 1:2:xtic(1) ls 3 pt 7  title "16 threads" w lp,\
	 "omp_32.txt" using 1:2:xtic(1) ls 4 pt 7  title "32 threads" w lp,\
