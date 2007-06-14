# gnuplot file

reset

# get a bunch of macros
load 'series.gp'

ll='w l ls'
set style line 1 lt 1 lw 3 
set style line 2 lt 2 lw 3 lc rgb "#008000"
set style line 3 lt 3 lw 3 


set yrange [0:0.045]

set xrange [150:200]
set grid

set key spacing 1.5
set size ratio 1

set xlabel 'reconstructed m_t [GeV/c^2]'
set ylabel '1/n dn/dm' offset 1

set label 3 'Pythia 6.325, m_t = 175 GeV/c^2' at graph 0.05,0.05

plan='\
set label 1 "Midpoint (f=0.75)" at graph 0.04,0.93;\
set label 4 "R=".sprintf("%3.1f",R) at graph 0.04,0.85;\
set label 5 "tt -> bqq+b{/Symbol mn_m}" at graph 0.04,0.77;\
set label 6 "Tevatron" at graph 0.04,0.69;\
plot \
     midpoint(R,0) u 2:5 @ll 1 t "no UE", \
     midpoint(R,1) u 2:5 @ll 2 t "with UE", \
     midpoint(R,1) u 2:13 @ll 3 t "<n_{PU}>=2.3"'

R=0.4
@plan
`gnupr midpoint_R_dep_R0.4.eps cld`

R=0.5
@plan
`gnupr midpoint_R_dep_R0.5.eps cld`

R=0.6
@plan
`gnupr midpoint_R_dep_R0.6.eps cld`

R=0.7
@plan
`gnupr midpoint_R_dep_R0.7.eps cld`

R=0.8
@plan
`gnupr midpoint_R_dep_R0.8.eps cld`

R=1.0
@plan
`gnupr midpoint_R_dep_R1.0.eps cld`

# set key width -3
# plan='\
# set label 1 "SISCone (f=0.75)" at graph 0.04,0.93;\
# set label 4 "R=".sprintf("%3.1f",R) at graph 0.04,0.85;\
# set label 5 "tt -> bqq+b{/Symbol mn_m}" at graph 0.04,0.77;\
# plot \
#      siscone(R,0) u 2:5 @ll 1 t "no UE", \
#      siscone(R,1) u 2:9 @ll 2 t "with UE (sub)", \
#      siscone(R,1) u 2:17 @ll 3 t "<n_{PU}>=2.3 (sub)"'
# 
# R=0.4
# @plan
# `gnupr siscone_R_dep_sub_R0.4.eps cld`
# 
# R=0.5
# @plan
# `gnupr siscone_R_dep_sub_R0.5.eps cld`
# 
# R=0.6
# @plan
# `gnupr siscone_R_dep_sub_R0.6.eps cld`
# 
# R=0.7
# @plan
# `gnupr siscone_R_dep_sub_R0.7.eps cld`
# 
# R=0.8
# @plan
# `gnupr siscone_R_dep_sub_R0.8.eps cld`
# 
# R=1.0
# @plan
# `gnupr siscone_R_dep_sub_R1.0.eps cld`
