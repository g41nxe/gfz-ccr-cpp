#!/usr/bin/gnuplot

date = system("date +%F-%H-%M-%S")
filename = "benchmark-".date.".png"

set output filename

set terminal png

set title "fft benchmark for different array sizes and and multiple runs"

set xlabel "# iterations"
set ylabel "time (s)"

set key on outside top right
set logscale y
set xrange [0:110]
set bmargin 4

set style line 1 lc rgb "#6B8E23"  lt 1 lw 1.5 ps 1 pi -1  # --- green
set style line 2 lc rgb "#104E8B"  lt 1 lw 1.5 ps 1 pi -1  # --- blue
set style line 3 lc rgb "#CD3333"  lt 1 lw 1.5 ps 1 pi -1  # --- red
set style line 4 lc rgb "#000000"  lt 1 lw 1.5 ps 1 pi -1  # --- black

plot "ite.txt"  using 1:2 ls 1 pt 5  title "iterative 512" w lp, \
     "ite.txt"  using 1:3 ls 1 pt 7  title "iterative 1024" w lp, \
     "ite.txt"  using 1:4 ls 1 pt 11 title "iterative 2048" w lp, \
     \
     "rec.txt"  using 1:2 ls 2 pt 5  title "recursive" w lp, \
     "rec.txt"  using 1:3 ls 2 pt 7  title "" w lp, \
     "rec.txt"  using 1:4 ls 2 pt 11 title "" w lp, \
     \
     "omp.txt" using 1:2 ls 3 pt 5  title "omp" w lp, \
     "omp.txt" using 1:3 ls 3 pt 7  title "" w lp, \
     "omp.txt" using 1:4 ls 3 pt 11 title "" w lp, \
	\
     "bru.txt" using 1:2 ls 4 pt 5  title "bruteforce" w lp, \
     "bru.txt" using 1:3 ls 3 pt 7  title "" w lp, \
     "bru.txt" using 1:4 ls 3 pt 11 title "" w lp
