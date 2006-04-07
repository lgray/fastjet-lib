# gnuplot file for examining the area of the kt and cambridge
# algorithms as a function of the separation between a soft and a hard
# parton

reset
set sty data li
kt_h(hlf_d) = 2*hlf_d*sqrt(1-hlf_d**2)+(pi-2*acos(hlf_d))
kt(d)=kt_h(d/2)
cam(d) = d<0.5 ? 0.0 : \
       -acos(0.5/d) + sqrt(d**2-0.25) + d**2*acos(1/(2*d**2)-1)

set xrange [0:1]
set yrange [0:2.5]

set key  left Left spacing 1.5
set xlabel 'dist (d) between partons'
set ylabel '{/Symbol D}A(d)/d'
plot kt(x)/x w l lt 1 lw 2, cam(x)/x w l lt 3 lw 2

