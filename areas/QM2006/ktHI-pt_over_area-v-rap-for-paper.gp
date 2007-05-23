# gnuplot file

reset

set ylabel 'P_{ti} / A_i [GeV]' offset 0.5
set xlabel 'y_i'

set size 1.0,0.8

set xrange [-5:5]
set yrange [00:450]

set ytics 100
set mytics 2
set xtics 2
#set mxtics 2

set label 1 "k_t, R=0.4" at graph 0.95,0.90 right
#set label 3 "FastJet" at graph 0.95,0.81 right

set label 11 "Hydjet, dN_{ch}/dy = 1600" at graph 0.05,0.15
set label 12 'with 2 hard jets, p_t {/Symbol \176} 100 GeV' at graph 0.05,0.05
#set label 3  "Z{/Symbol \242} at 2 TeV"   at graph 0.035,0.70

set label 21 "LHC, Pb Pb" at graph 0.05,0.90
set label 22 '{/Symbol \326}s = 5.5 TeV' at graph 0.05,0.80

set grid noxtics noytics front



plot 'HIevent-LHC-Nch1600-ktjets-discard0.0.dat' u 2:($4/$5) w p pt 7 ps 2 t ''
replot 238-4.56*x**2 w l lt 3 lw 3 t ''

`gnupr ktHI-pt_over_area-v-rap-for-paper.eps col` 

# 
# 
# 
# set label 10 "Hard event" at graph 0.05,0.95
# 
# set yrange [-20:120]
# 
# set xzeroaxis
# 
# 
# 
# plot 'HIevent-LHC-Nch1600-hardev-ktjets-discard0.0.dat' u 2:4 w p pt 7 ps 2 t ''
# `gnupr ktsequence-hard.eps col`
# pause pauseinterval
# 
# #----------------------------------------
# set yrange [0:300]
# replot
# `gnupr ktsequence-hard-bigscale.eps col`
# pause pauseinterval
# 
# #----------------------------------------
# set label 10 "Hard event in PbPb (LHC)"
# plot 'HIevent-LHC-Nch1600-ktjets-discard0.0.dat' u 2:4 w p pt 7 ps 2 t ''
# `gnupr ktsequence-full.eps col`
# pause pauseinterval
# 
# 
# #----------------------------------------
# set label 11 "P_t density in jets" at graph 0.05,0.87
# set ylabel 'P_t / Area [GeV]'
# set yrange [0:500]
# plot 'HIevent-LHC-Nch1600-ktjets-discard0.0.dat' u 2:($4/$5) w p pt 7 ps 2 t ''
# `gnupr ktsequence-full-ptOverA.eps col`
# pause pauseinterval
# 
# 
# #----------------------------------------
# set label 20 "Fit: (238 - 4.6 y^2) GeV" at graph 0.9,0.1 right
# replot 238-4.56*x**2 w l lt 3 lw 3 t ''
# `gnupr ktsequence-full-ptOverA+fit.eps col`
# unset label 20
# pause pauseinterval
# 
# 
# #----------------------------------------
# set label 11 "Subtracted P_t density"
# set yrange [-100:400]
# plot 'HIevent-LHC-Nch1600-ktjets-discard0.0.dat' u 2:($10/$5) w p pt 7 ps 2 t ''
# `gnupr ktsequence-full-ptOverA-subtracted.eps col`
# pause pauseinterval
# 
# 
# #----------------------------------------
# set label 11 "Subtracted P_t"
# set ylabel 'P_t [GeV]'
# set yrange [-20:120]
# plot 'HIevent-LHC-Nch1600-ktjets-discard0.0.dat' u 2:($10) w p pt 7 ps 2 t ''
# `gnupr ktsequence-full-subtracted.eps col`
# #pause pauseinterval





