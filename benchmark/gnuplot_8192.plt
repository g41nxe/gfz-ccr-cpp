#!/usr/bin/gnuplot

date = system("date +%F-%H-%M-%S")
filename = "benchmark_8192-".date.".png"

set output filename

set terminal png

set title "ccr benchmark for array size 8192"

set xlabel "# cross correlations"
set ylabel "time (s)"

set key on outside top right
#set logscale y
set xrange [50:350]
set bmargin 4

set style line 1 lc rgb "#6B8E23"  lt 1 lw 1.5 ps 1 pi -1  # --- green
set style line 2 lc rgb "#104E8B"  lt 1 lw 1.5 ps 1 pi -1  # --- blue
set style line 3 lc rgb "#CD3333"  lt 1 lw 1.5 ps 1 pi -1  # --- red
set style line 4 lc rgb "#000000"  lt 1 lw 1.5 ps 1 pi -1  # --- black

plot "ite.txt"  using 1:3 ls 1 pt 7  title "iterative" w lp, \
    
     "rec.txt"  using 1:3 ls 2 pt 7  title "recursive" w lp, \
     
     "omp.txt" using 1:3 ls 3 pt 7  title "openmp" w lp, \
   
     "bru.txt" using 1:3 ls 3 pt 7  title "bruteforce" w lp, \
