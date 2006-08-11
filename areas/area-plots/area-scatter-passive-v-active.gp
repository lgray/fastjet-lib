# gnuplot file

reset
set dat st li

# plot jets with |rapidities| less than 4
set xrange [0:10]
set yrange [0:10]
set xlabel 'active area'
set ylabel 'passive area (rfact=0.9)'
set label 1 "k_t algorithm, R=1"              at 9.5,3 right
set label 2 "10 LHC events (p_t c. 1TeV)"     at 9.5,2 right
set label 3 "jets with |y| < 4, p_t > 10 GeV" at 9.5,1 right
set size square
set key left Left reverse spacing 1.5
plot '<paste area-scatter-active-repeat15 area-scatter-passive-rfact0.9' \
     u (abs($2)<4?$5:-1):14 w p t 'measured areas',\
     x t ''


