#gnuplot file

reset
set xlabel 'R'
set ylabel '{/Symbol r \261 s} [GeV]'

set grid noxtics noytics front

plot 'rho_sigma_v_R.res' u 1:($2-$3):($2+$3) w filledcurves lt 6 t ''
replot 'rho_sigma_v_R.res' u 1:2 w l lw 3 lt 3 t ''

#plot 'rho_sigma_v_R.res' u 1:(1-$3/$2):(1+$3/$2) w filledcurves lt 6 t ''
#replot 1 w l lw 3 lt 3 t ''
