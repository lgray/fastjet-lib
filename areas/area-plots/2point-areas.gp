# gnuplot file


reset
set dat sty li

set yrange [0:1.8]
set xrange [0:2.1]

set xlabel '{/Symbol Dh}'
set ylabel 'Area / {/Symbol p}R^2'

set key spacing 1.5

# raw kt area according to CCN27-76
fktraw(d) = d*sqrt(1-d**2/4.0) + 2*(pi - acos(d/2))

# it gets converted into a deficit by taking the effective kt area formula
# and dividing by two
fdeficit(d) = (d<2&&d>=1) ? 0.5*fktraw(d) : pi

#  kt area according to CCN27-76
fkt(d) = (d<1) ?  fktraw(d) : fdeficit(d)

# cam area according to CCN27-79
fcam(d) = (d<1 && d>0.5) ? \
        pi-acos(1.0/(2*d))+sqrt(d**2-0.25)+d**2*acos(1/(2*d**2)-1) \
        : fdeficit(d)


  plot fkt(x)/pi             w l lt 1 lw 2 t 'kt passive area'
replot 'kt-2point-area-nrep100k.res'   u 1:($3/pi)  w l lt 2 lw 2 t 'kt active area'

replot fcam(x)/pi            w l lt 3 lw 2 t 'cam passive area'
replot 'cam-2point-area-nrep100k.res' u 1:($3/pi)    w l lt 4 lw 2 t 'cam active area'
