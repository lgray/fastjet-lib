
plot 'areas-hist.dat' i 0 u 1:2:3 w yerr t 'cam'
replot 'areas-hist.dat' i 2 u 1:2:3 w yerr t 'cam, etamax=3'
replot 'areas-hist.dat' i 3 u 1:2:3 w yerr t 'cam, etamax=100'
replot 'areas-hist.dat' i 4 u 1:2:3 w yerr t 'cam, etamax=100, pt_scheme'
replot 'areas-hist.dat' i 5 u 1:2:3 w yerr t 'cam, etamax=1'

reset
replot 'areas-hist.dat' i 0 u 1:2 w l lt 0 t ''
replot 'areas-hist.dat' i 2 u 1:2 w l lt 0 t ''
replot 'areas-hist.dat' i 3 u 1:2 w l lt 0 t ''
replot 'areas-hist.dat' i 4 u 1:2 w l lt 0 t ''
replot 'areas-hist.dat' i 5 u 1:2 w l lt 0 t ''
