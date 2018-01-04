#!/usr/bin/gnuplot
set logscale xz
set xlabel "NMAX"
set ylabel "Time(s)"
set grid
set title "Dot Product Parallelization Performance Scaling "
set terminal postscript color "Helvetica" 14
set output "perf.ps"
plot 'perf.dat' using 1:2 with linespoint title "Serial ", \
	 'perf.dat' using 1:3 with linespoint title "Parallel (Red)[1 Thread(s)]", \
	 'perf.dat' using 1:4 with linespoint title "Parallel (Crit)[1 Thread(s)]", \
 	 'perf.dat' using 1:5 with linespoint title "Parallel (Red)[2 Thread(s)]", \
	 'perf.dat' using 1:6 with linespoint title "Parallel (Crit)[2 Thread(s)]", \
 	 'perf.dat' using 1:7 with linespoint title "Parallel (Red)[4 Thread(s)]", \
	 'perf.dat' using 1:8 with linespoint title "Parallel (Crit)[4 Thread(s)]", \
 	 'perf.dat' using 1:9 with linespoint title "Parallel (Red)[8 Thread(s)]", \
	 'perf.dat' using 1:10 with linespoint title "Parallel (Crit)[8 Thread(s)]", \


set logscale xz
set xlabel "#Threads"
set ylabel "Scaling Efficiency (Speedup ratio) "
set grid
set title "Dot Product Parallelization Performance Efficiency"
set terminal postscript color "Helvetica" 14
set output "perf_scale.ps"
plot 'perf_eff.dat' using 1:2 with linespoint title "Parallel (Crit) 10^5", \
	 'perf_eff.dat' using 1:3 with linespoint title "Parallel (Red) 10^5", \
	 'perf_eff.dat' using 1:4 with linespoint title "Parallel (Crit) 10^6", \
	 'perf_eff.dat' using 1:5 with linespoint title "Parallel (Red) 10^6 ", \
	 'perf_eff.dat' using 1:6 with linespoint title "Parallel (Crit) 10^7", \
	 'perf_eff.dat' using 1:7 with linespoint title "Parallel (Red) 10^7", \
	 'perf_eff.dat' using 1:8 with linespoint title "Parallel (Crit) 10^8", \
	 'perf_eff.dat' using 1:9 with linespoint title "Parallel (Red) 10^8", \


