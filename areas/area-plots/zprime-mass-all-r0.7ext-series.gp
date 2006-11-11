# gnuplot file

reset
#set xrange [1800:2300]
set xrange [1850:2200]
set dat sty li

set xlabel "reconstructed Z' mass [GeV]"
set ylabel '1/N dN/dmass' 1,0

set key spacing 1.5



binsize=2
binshift=binsize*0.5

ymax=0.012
set yrange [0:ymax]

set arrow 1 from 2e3,0 to 2e3,ymax nohead lt 0
set label 1 'R=0.7, LHC' at graph 0.9,0.4 right


plot '../mass-tests/zprime-highlumi-r0.7.res' u ($1+binshift):2 w fs lt 2 lw 2 t 'kt, no pileup'
replot '../mass-tests/zprime-highlumi-r0.7.res' u ($1+binshift):4 w fs lt 3 lw 2 t 'kt + high-lumi'

`gnupr zprime-mass-all-r0.7ext-series-01.eps col`

replot '../mass-tests/zprime-cone-highlumi-r0.7-all.res' u ($1+binshift):4 w fs lt 4 lw 2 t 'cone + high-lumi'

`gnupr zprime-mass-all-r0.7ext-series-02.eps col`

replot '../mass-tests/zprime-camarea-highlumi-r0.7.res' u ($1+binshift):4 w fs lt 5 lw 2 t 'cam + high-lumi'
`gnupr zprime-mass-all-r0.7ext-series-03.eps col`

replot '../mass-tests/zprime-camarea-highlumi-r0.7ext.res' u ($1+binshift):7 w fs lt -1 lw 3 t '- correction'
#replot '../mass-tests/zprime-highlumi-r0.7ext.res' u ($1+binshift):7 w fs lt -1 lw 1 t '- correction'

`gnupr zprime-mass-all-r0.7ext-series-04.eps col`

#replot '../mass-tests/zprime-camarea-highlumi-r0.7ext.res' u ($1+binshift):7 w fs lt 5 lw 2 t 'cam + mult-interactions (SUBTRCTD)'
