# gnuplot file

reset

#set grid xtics
set yzeroaxis

set key spacing 1.5
set xlabel '{/Symbol D}R  (reconstructed - true)'
set ylabel '1/N{/Symbol D}R  dN/d{/Symbol D}R' 0.8
set size square

#set ytics 0.0,0.01

set label 1 "dN_{ch}/dy = 1500" at graph 0.30,0.32
set label 3 "Jets with 90 < P_t < 100 GeV" at graph 0.30,0.24

#set xrange [-30:30]
set xrange [0:0.2]

set label 10 'k_t R=0.4' at -17,0.027 right tc lt 1
set label 20 'Aachen/Cam' at 7,0.022 tc lt 3
set label 21 'R=0.4'      at 7,0.0195 tc lt 3

plot '../hydjet_eff/hydjetDEF_nhsel1_ptminhard50_kt_r0.4_maxrapphi0.2.res' i 29 u ($1-0.0005):($4/$2/0.008) w st lt 1 lw 2 t 'kt R=0.4'
replot '../hydjet_eff/hydjetDEF_nhsel1_ptminhard50_cam_r0.4_maxrapphi0.2.res' i 29 u ($1+0.0005):($4/$2/0.008) w st lt 3 lw 2 t 'Aachen/Cam R=0.4'

