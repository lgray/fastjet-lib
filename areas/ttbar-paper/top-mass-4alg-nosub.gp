# gnuplot file

reset

set yrange [0:0.02]
set xrange [140:200]

#set grid

#plot 'antikt_R0.4_RUK_bin2_long.res' u 2:5 w histeps,\
#     'antikt_R0.4_RUK_bin2_long.res' u 2:5 w histeps,\
#     'antikt_R0.5_RUK_bin2_long.res' u 2:5 w histeps,\
#     'antikt_R0.6_RUK_bin2_long.res' u 2:5 w histeps

set size square
set key spacing 1.5
set key left Left reverse width -10

set ylabel '1/N dN/dm [GeV^{-1}]' offset 2,0
set xlabel 'reconstructed top mass [GeV]'

set label 2 '{/*0.8 LHC (no pileup)}' at graph 0.035,0.05 
set label 1 '{/*0.8 m_t = 175 GeV}'   at graph 0.965,0.045 right
set label 11 '{/*0.8 Pythia}'         at graph 0.965,0.10 right
set label 3 'R=0.5'           at graph 0.965,0.95 right

plot 'kt_R0.5_bin2_long.res'  u 2:5 w l lt 2lw 3  t 'k_t',\
     'cam_R0.5_bin2_long.res' u 2:5 w l lt 3 lw 3 t 'Cam/Aachen',\
     'siscone_R0.5_f0.5_passive_RUK_bin2_long.res' u 2:5 w l lt 4 lw 3 t 'SISCone (f=0.5)',\
     'antikt_R0.5_RUK_bin2_long.res' u 2:5 w l lt 1 lw 3 t 'anti-k_t'
