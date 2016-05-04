# plot.gp
set logscale x 10
set xrange [100000:20000000]
set format x "%1.0e"
set style line 1 lt 1 lc rgbcolor "#0072b2" lw 3 pt 7 ps 1.2
set style line 2 lt 1 lc rgbcolor "#e51e10" lw 3 pt 7 ps 1.2
set style line 3 lt 1 lc rgbcolor "#009e73" lw 3 pt 7 ps 1.2
set style line 4 lt 1 lc rgbcolor "#a020f0" lw 3 pt 7 ps 1.2
plot "sample.dat" u 1:2 w lp ls 1 t "C++"
replot "sample.dat" u 1:3 w lp ls 2 t "Go"
replot "sample.dat" u 1:4 w lp ls 3 t "Java"
replot "sample.dat" u 1:5 w lp ls 4 t "Python"
set terminal png
set output "sample.png"
replot
