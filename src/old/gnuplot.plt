set title "cross-correlation benchmark"
set xlabel "# iterations"
set ylabel "time (s)"
set key on inside bottom right
set logsacle y
set xrange [40:410]
set bmargin 4

set style line 1 lc rgb "#6B8E23"  lt 1 lw 1.5 ps 1 pi -1  # --- blue
set style line 2 lc rgb "#104E8B"  lt 1 lw 1.5 ps 1 pi -1  # --- red
set style line 3 lc rgb "#CD3333"  lt 1 lw 1.5 ps 1 pi -1  # --- green

plot "iterative.txt"  using 1:2 ls 1 pt 5  title "fft (it) 1024" w lp, \
     ""               using 1:3 ls 1 pt 7  title "fft (it) 2048" w lp, \
     ""               using 1:4 ls 1 pt 11 title "fft (it) 4096" w lp, \
     "recursive.txt"  using 1:2 ls 2 pt 5  title "fft (rec)" w lp, \
     ""               using 1:3 ls 2 pt 7  title "" w lp, \
     ""               using 1:4 ls 2 pt 11 title "" w lp, \
     "bruteforce.txt" using 1:2 ls 3 pt 5  title "bruteforce" w lp, \
     ""               using 1:3 ls 3 pt 7  title "" w lp, \
     ""               using 1:4 ls 3 pt 11 title "" w lp