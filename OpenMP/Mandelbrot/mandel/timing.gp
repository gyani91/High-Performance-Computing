set terminal postscript color "Helvetica" 14
set boxwidth 0.75
set style fill solid

set xlabel "Number of Threads"
set ylabel "Time(ms)"
set grid
set title "Mandelbrot Set Parallel Scaling Efficiency"


set output "timing.ps"
plot 'timing.dat' using 2:xtic(1) with boxes lt rgb "#1E90FF"  title "Execution Time(ms)"