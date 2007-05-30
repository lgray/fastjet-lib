# gnuplot file

reset

# get a bunch of macros
load 'series.gp'

set yrange [0:0.035]

set xrange [150:190]
set grid

set xlabel 'reconstructed m_t [GeV/c^2]'
set ylabel '1/n dn/dm' offset 1


set label 3 'Pythia 6.325, UE on, m_t = 175 GeV/c^2' at graph 0.05,0.05


mp=series("midpoint",1,5,1)
sc=series("siscone",1,5,2)

plot @mp,@sc
