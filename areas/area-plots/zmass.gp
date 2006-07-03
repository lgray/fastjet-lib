# gnuplot file

reset
set dat st li
set xrange [0:250]

set xlabel 'reconstructed Z mass [GeV]'
set ylabel '1/N dN/dmass' 1,0

set key spacing 1.5

binsize=2
binshift=binsize*0.5

set label 1 'kt algorithm (D=1.0)' at graph 0.9,0.4 right
ymax=0.045
set yrange [0:ymax]
set arrow 1 from 91.2,0 to 91.2,ymax nohead lt 0
set ytics 0,0.01

  plot '../mass-tests/zerolumi-noMI-r1.0.res' u ($1+binshift):2 w fs lt 1 lw 2 t 'no mult-interactions'
replot '../mass-tests/highlumi-r1.0.res'      u ($1+binshift):2    w fs lt 2 lw 2 t  '+ mult-interactions'
replot '../mass-tests/highlumi-r1.0.res'      u ($1+binshift):4    w fs lt 3 lw 2 t  '+ min-bias (100 fb^{-1}/yr)'
replot '../mass-tests/highlumi-r1.0.res'      u ($1+binshift):5    w fs lt -1 lw 3 t  '{/Symbol -} correction'

`gnupr zmass-kt-d1.0.eps col`



set label 1 'kt algorithm (D=0.7)'
ymax=0.045
set yrange [0:ymax]
set arrow 1 from 91.2,0 to 91.2,ymax nohead lt 0
set ytics 0,0.01


  plot '../mass-tests/zerolumi-noMI-r0.7.res' u ($1+binshift):2 w fs lt 1 lw 2 t 'no mult-interactions'
replot '../mass-tests/highlumi-r0.7.res'      u ($1+binshift):2    w fs lt 2 lw 2 t  '+ mult-interactions'
replot '../mass-tests/highlumi-r0.7.res'      u ($1+binshift):4    w fs lt 3 lw 2 t  '+ min-bias (100 fb^{-1}/yr)'
replot '../mass-tests/highlumi-r0.7.res'      u ($1+binshift):5    w fs lt -1 lw 3 t  '{/Symbol -} correction'

`gnupr zmass-kt-d0.7.eps col`


set label 1 'kt algorithm (D=0.4)' 
ymax=0.045
set yrange [0:ymax]
set arrow 1 from 91.2,0 to 91.2,ymax nohead lt 0
set ytics 0,0.01

  plot '../mass-tests/zerolumi-noMI-r0.4.res' u ($1+binshift):2 w fs lt 1 lw 2 t 'no mult-interactions'
replot '../mass-tests/highlumi-r0.4.res'      u ($1+binshift):2    w fs lt 2 lw 2 t  '+ mult-interactions'
replot '../mass-tests/highlumi-r0.4.res'      u ($1+binshift):4    w fs lt 3 lw 2 t  '+ min-bias (100 fb^{-1}/yr)'
replot '../mass-tests/highlumi-r0.4.res'      u ($1+binshift):5    w fs lt -1 lw 3 t  '{/Symbol -} correction'

`gnupr zmass-kt-d0.4.eps col`


set label 1 'ILCA cone (R=0.7)' 
set label 2 '(f_{overlap} = 0.50)'   at graph 0.9,0.31 right
ymax=0.045
set yrange [0:ymax]
set arrow 1 from 91.2,0 to 91.2,ymax nohead lt 0
set ytics 0,0.01

  plot '../mass-tests/cone-zerolumi-noMI-r0.7.res' u ($1+binshift):2 w fs lt 1 lw 2 t 'no mult-interactions'
replot '../mass-tests/cone-highlumi-r0.7-all.res'      u ($1+binshift):2    w fs lt 2 lw 2 t  '+ mult-interactions'
replot '../mass-tests/cone-highlumi-r0.7-all.res'      u ($1+binshift):4    w fs lt 3 lw 2 t  '+ min-bias (100 fb^{-1}/yr)'

`gnupr zmass-cone-r0.7.eps col`




set label 1 'ILCA cone (R=0.4)' 
set label 2 '(f_{overlap} = 0.50)'
ymax=0.045
set yrange [0:ymax]
set arrow 1 from 91.2,0 to 91.2,ymax nohead lt 0
set ytics 0,0.01

  plot '../mass-tests/cone-zerolumi-noMI-r0.4.res' u ($1+binshift):2 w fs lt 1 lw 2 t 'no mult-interactions'
replot '../mass-tests/cone-highlumi-r0.4-all.res'      u ($1+binshift):2    w fs lt 2 lw 2 t  '+ mult-interactions'
replot '../mass-tests/cone-highlumi-r0.4-all.res'      u ($1+binshift):4    w fs lt 3 lw 2 t  '+ min-bias (100 fb^{-1}/yr)'

`gnupr zmass-cone-r0.4.eps col`



set label 1 'ILCA cone (R=0.7)' 
set label 2 '(f_{overlap} = 0.75)'
ymax=0.045
set yrange [0:ymax]
set arrow 1 from 91.2,0 to 91.2,ymax nohead lt 0
set ytics 0,0.01

#  plot '../mass-tests/cone-zerolumi-noMI-r0.7.res' u ($1+binshift):2 w fs lt 1 lw 2 t 'no mult-interactions'
  plot '../mass-tests/cone075-highlumi-r0.7-all.res'      u ($1+binshift):2    w fs lt 2 lw 2 t  '+ mult-interactions'
replot '../mass-tests/cone075-highlumi-r0.7-all.res'      u ($1+binshift):4    w fs lt 3 lw 2 t  '+ min-bias (100 fb^{-1}/yr)'

`gnupr zmass-cone075-r0.7.eps col`



set label 1 'R=0.7, high lumi'
unset label 2
ymax=0.045
set yrange [0:ymax]
set arrow 1 from 91.2,0 to 91.2,ymax nohead lt 0
set ytics 0,0.01


plot '../mass-tests/highlumi-r0.7.res'              u ($1+binshift):4    w fs lt 3 lw 2 t  'k_t',\
     '../mass-tests/cone-highlumi-r0.7-all.res'     u ($1+binshift):4    w fs lt 4 lw 2 t  'ILCA cone (f = 0.50)',\
     '../mass-tests/cone075-highlumi-r0.7-all.res'  u ($1+binshift):4    w fs lt 5 lw 2 t  'ILCA cone (f = 0.75)'

`gnupr zmass-kt-cone-r0.7.eps col`
