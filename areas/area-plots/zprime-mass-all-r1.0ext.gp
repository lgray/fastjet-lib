# gnuplot file

reset
set xrange [1800:2300]
set dat sty li

set xlabel 'reconstructed Z mass [GeV]'
set ylabel '1/N dN/dmass' 1,0

set key spacing 1.5


set grid

binsize=2
binshift=binsize*0.5


plot '../mass-tests/zprime-camarea-highlumi-r1.0.res' u ($1+binshift):2 w fs lt 1 lw 2 t 'cam, no mult-interactions'
replot '../mass-tests/zprime-highlumi-r1.0.res' u ($1+binshift):4 w fs lt 4 lw 2 t 'kt + mult-interactions'
replot '../mass-tests/zprime-highlumi-r1.0ext.res' u ($1+binshift):5 w fs lt 2 lw 2 t 'kt + mult-interactions (SUB)'
replot '../mass-tests/zprime-highlumi-r1.0ext.res' u ($1+binshift):7 w fs lt 6 lw 2 t 'kt + mult-interactions (EXT-SUB)'


#replot '../mass-tests/zprime-camarea-highlumi-r1.0.res' u ($1+binshift):4 w fs lt 3 lw 2 t 'cam + mult-interactions'
#replot '../mass-tests/zprime-camarea-highlumi-r1.0.res' u ($1+binshift):5 w fs lt 5 lw 2 t 'cam + mult-interactions (SUBTRCTD)'
#replot '../mass-tests/zprime-cone-highlumi-r1.0-all.res' u ($1+binshift):4 w fs lt 2 lw 2 t 'cone + mult-interactions'
