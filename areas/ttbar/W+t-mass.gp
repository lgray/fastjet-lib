# gnuplot file

reset
set dat st li
set xrange [20:250]

set xlabel 'reconstructed W / top mass [GeV]'
set ylabel '1/N dN/dmass' 1,0

set key spacing 1.5

binsize=2
binshift=binsize*0.5


set label 1 'R=0.4, LHC'   at graph 0.035,0.93 
set label 2 'semileptonic' at graph 0.035,0.86
set label 3 'ttbar events' at graph 0.035,0.79
ymax=0.0265
set yrange [0:ymax]
set arrow 1 from 80.45,0 to 80.45,ymax nohead lt 0
set arrow 2 from 175,0   to 175.0,ymax nohead lt 0
set ytics 0,0.01
set mxtics 5
set mytics 2

set label 10 "W mass"   at 60,0.013 right
set label 11 "top mass" at 200,0.01 

below(x,lim,y)= x<=lim ? y : -5
above(x,lim,y)= x>=lim ? y : -5

plot 'highlumi_kt_r0.4' \
        u 1:(below($1,115,$4))    w st lt 2 lw 2 t  'k_t, no pileup',\
    ''  u 1:(above($1,135,$5))    w st lt 2 lw 2 t  '',\
    ''  u 1:(below($1,115,$12))   w st lt 3 lw 2 t  'k_t, pileup',\
    ''  u 1:(above($1,135,$13))   w st lt 3 lw 2 t  '',\
    ''  u 1:(below($1,115,$16))   w st lt -1 lw 3 t  'k_t, pileup, corrected',\
    ''  u 1:(above($1,135,$17))   w st lt -1 lw 3 t  ''
`gnupr W+t-mass-kt.eps col`

plot 'highlumi_cam_r0.4' \
        u 1:(below($1,115,$4))    w st lt 2 lw 2 t  'cam, no pileup',\
    ''  u 1:(above($1,135,$5))    w st lt 2 lw 2 t  '',\
    ''  u 1:(below($1,115,$12))   w st lt 3 lw 2 t  'cam, pileup',\
    ''  u 1:(above($1,135,$13))   w st lt 3 lw 2 t  '',\
    ''  u 1:(below($1,115,$16))   w st lt -1 lw 3 t  'cam, pileup, corrected',\
    ''  u 1:(above($1,135,$17))   w st lt -1 lw 3 t  ''
`gnupr W+t-mass-cam.eps col`
