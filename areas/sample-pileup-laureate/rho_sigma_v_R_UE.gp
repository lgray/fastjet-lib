#gnuplot file

reset
set xlabel 'R'
set ylabel '{/Symbol r}_{UE} {/Symbol \261 s}_{UE} [GeV]'


raprange=8

njets(yrange,mnarea)=(6.28*yrange/mnarea)
rhoerr(err,yrange,mnarea)=err*sqrt(mnarea)/sqrt(njets(yrange,mnarea))


set yrange [0:7]
set grid noxtics noytics front

set label 1 '1 Pythia event (with UE)' at 0.1,6.5
set label 2 'LHC dijet (pt c. 100 GeV)' at 0.1,5.7

plot 'rho_sigma_v_R_UE.res' u 1:($2-$3):($2+$3) w filledcurves lt 6 t ''
replot 'rho_sigma_v_R_UE.res' u 1:($2-rhoerr($3,raprange,$4)):($2+rhoerr($3,raprange,$4)) w filledcurves lt 2 t ''
replot 'rho_sigma_v_R_UE.res' u 1:2 w l lw 3 lt 3 t ''

#plot 'rho_sigma_v_R.res' u 1:(1-$3/$2):(1+$3/$2) w filledcurves lt 6 t ''
#replot 1 w l lw 3 lt 3 t ''
