#!/usr/bin/gnuplot

date = system("date +%F-%H-%M-%S")
filename = "ccr-".date.".eps"

set output filename


set terminal epscairo enhanced color size 8,6

set multiplot layout 2, 1
set key samplen 2 spacing 1.5 font "Arial, 16"

set xtics font "Arial, 12"
set ytics font "Arial, 12"

set title font "Arial-bold, 20"

set xlabel font "Arial, 12" 
set ylabel font "Arial, 12"

set style line 1 lc rgb "#6B8E23"  lt 1 lw 1.75 ps 0.5 pi -1  # --- green
set style line 2 lc rgb "#104E8B"  lt 1 lw 1.75 ps 0.5 pi -1  # --- blue
set style line 3 lc rgb "#CD3333"  lt 1 lw 1.75 ps 1 pi -1  # --- red

set xrange [-1:32]
set yrange [-1:1]
set xtics 0,2,31
set ytics -1,0.25,1
set xtics scale 0.5
set ytics scale 0.5

set grid

set key vertical box above left height 1.5 width 0

set lmargin 5
set bmargin 3

set title "Testdatensatz"

plot "ccr2.txt"  index 0 using 1:2 ls 1 pt 7 w lp \
     title 'f(t) = 0.75 {/Symbol \327} sin(4{/Symbol \327}{/Symbol p}{/Symbol \327}1/32{/Symbol \327}t)', \
     "ccr2.txt"  index 0 using 1:3 ls 2 pt 7 w lp \
     title 'g(t) = 0.75 {/Symbol \327} sin(4{/Symbol \327}{/Symbol p}{/Symbol \327}1/32{/Symbol \327}t+{/Symbol p})'

set xrange [-32:32]
set yrange[-10:10]
set ytics -9,3,8
set xtics -32,4,32

set boxwidth 0.2
set style fill solid

set title "Kreuzkorrelation"

plot "ccr2.txt"  index 1 using 1:2 ls 3 pt 7 w boxes \
     title  'ccr( f(t) , g(t) )' #,\
#    "ccr2.txt"  index 2 using 1:2 ls 3 pt 7 w boxes

unset multiplot