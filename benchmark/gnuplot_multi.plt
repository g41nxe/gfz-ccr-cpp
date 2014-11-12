#!/usr/bin/gnuplot

date = system("date +%F-%H-%M-%S")
filename = "benchmark_multi-".date.".png"
set output filename
set terminal png size 800,640

set style line 1 lc rgb "#6B8E23"  lt 1 lw 1.5 ps 1 pi -1  # --- green
set style line 2 lc rgb "#104E8B"  lt 1 lw 1.5 ps 1 pi -1  # --- blue
set style line 3 lc rgb "#CD3333"  lt 1 lw 1.5 ps 1 pi -1  # --- red
set style line 4 lc rgb "#000000"  lt 1 lw 1.5 ps 1 pi -1  # --- black

set multiplot layout 1,3
set key samplen 2 spacing 0.7 font "Times-Roman, 9"
set key outside bottom center box

set xtics font "Times-Roman, 9"
set ytics font "Times-Roman, 9"
set title font "Times-Roman, 12"
set xlabel font "Times-Roman, 12" 
set ylabel font "Times-Roman, 12"

set xlabel "array size" 
set ylabel "time (ms)" offset 5
set logscale y
set xrange [100:1100]
set grid

plot "ite.txt"  using 1:3:xtic(1) ls 1 pt 7  title "iterative" w lp,\
     "rec.txt"  using 1:3:xtic(1) ls 2 pt 7  title "recursive" w lp,\
     "omp.txt"  using 1:3:xtic(1) ls 3 pt 7  title "parallel" w lp,\
     "bru.txt"  using 1:3:xtic(1) ls 4 pt 7  title "bruteforce" w lp,\


set ylabel "time (ms)" offset 5
set logscale y
set xrange [2500:18000]
set grid

plot "array.txt" index 0 using 1:2:xtic(1) ls 1 pt 7  title "iterative " w lp,\
     ""			 index 1 using 1:2:xtic(1) ls 2 pt 7  title "recursive" w lp,\
     ""			 index 2 using 1:2:xtic(1) ls 3 pt 7  title "parallel" w lp,\
     ""			 index 3 using 1:2:xtic(1) ls 4 pt 7  title "bruteforce" w lp

set xlabel "array size" 
set ylabel "time (ms)" offset 2
unset logscale y
set xrange [200:1050]
set grid

plot "omp_4.txt"  using 1:2:xtic(1) ls 1 pt 7  title "4 threads " w lp,\
	 "omp_8.txt"  using 1:2:xtic(1) ls 2 pt 7  title "8 threads" w lp,\
	 "omp_16.txt" using 1:2:xtic(1) ls 3 pt 7  title "16 threads" w lp,\
	 "omp_32.txt" using 1:2:xtic(1) ls 4 pt 7  title "32 threads" w lp,\



unset multiplot