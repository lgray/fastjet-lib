# gnuplot file

reset

# get a bunch of macros
load 'series.gp'

ll='w l lw 2 lt'

set yrange [0:0.035]

set xrange [150:190]
set grid

set key spacing 1.5
set size ratio 1

set xlabel 'reconstructed m_t [GeV/c^2]'
set ylabel '1/n dn/dm' offset 1

set label 3 'Pythia 6.325, UE on, m_t = 175 GeV/c^2' at graph 0.05,0.05

plan='\
set label 1 "Midpoint" at graph 0.04,0.93;\
set label 4 "R=".sprintf("%3.1f",R).", f=0.75" at graph 0.04,0.85;\
plot \
     midpoint05(R,ue) u 2:5 @ll 1 t "s = 0.5 GeV", \
     midpoint  (R,ue) u 2:5 @ll 2 t "s = 1.0 GeV",\
     midpoint20(R,ue) u 2:5 @ll 3 t "s = 2.0 GeV"'

#midpoint01(R,ue) u 2:5 @ll 4 t "top seed = 0.1 GeV", \

ue=1
R=0.4
@plan
`gnupr midpoint_seed_dep_R0.4.eps cld`

R=0.5
@plan
`gnupr midpoint_seed_dep_R0.5.eps cld`

R=0.6
@plan
`gnupr midpoint_seed_dep_R0.6.eps cld`

R=0.8
@plan
`gnupr midpoint_seed_dep_R0.8.eps cld`

R=1.0
@plan
`gnupr midpoint_seed_dep_R1.0.eps cld`


#ll='w l lw 1 lt'
#
#resc=1.39
#replot \
#     midpoint05(R,ue) u ($2/mW_mt):(resc*$4*mW_mt) @ll 1 t 'W seed = 0.5 GeV', \
#     midpoint  (R,ue) u ($2/mW_mt):(resc*$4*mW_mt) @ll 2 t 'W seed = 1.0 GeV',\
#     midpoint20(R,ue) u ($2/mW_mt):(resc*$4*mW_mt) @ll 3 t 'W seed = 2.0 GeV'
