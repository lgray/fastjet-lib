# gnuplot file

reset
set st dat li

xmin=40
xmax=220
set xrange  [xmin:xmax]
set x2range [xmin:xmax]

unset multiplot
filename="W+t-mass-3alg.eps"
set term postscript enhance color dashed portrait size 15cm,20cm
set output filename

set ylabel '1/N dN/dm [GeV^{-1}]' offset 2,0

set key spacing 1.5

binsize=2
binshift=binsize*0.5


#set label 3 'ttbar events' at graph 0.035,0.79
ymax=0.025
set yrange [0:ymax]
yarr=0.16*ymax
ybrr=0.05*ymax
#set arrow 1 from 80.45,yarr to 80.45,ybrr lt 0
#set arrow 2 from 175,yarr   to 175.0,ybrr lt 0
set ytics 0,0.01
set mxtics 4
set mytics 2

set label 10 "W"   at 60,0.013 right
set label 11 "top" at 180,0.012 

below(x,lim,y)= x<=lim ? y : -5
above(x,lim,y)= x>=lim ? y : -5


set macros

R="0.4"

kt="kt_R".R.".res"
cam="cam_R".R.".res"
cone="siscone_R0.4_f0.5_passive_RUC.res"


plotobj="\
        u 1:(below($1,115,$4))    w st lt 2 lw 1 t  @nopile,\
    ''  u 1:(above($1,135,$5))    w st lt 2 lw 1 t  '',\
    ''  u 1:(below($1,115,$8))    w st lt 1 lw 3 t  @nopilesub,\
    ''  u 1:(above($1,135,$9))    w st lt 1 lw 3 t  '',\
    ''  u 1:(below($1,115,$12))   w st lt 3 lw 1 t  @pile,\
    ''  u 1:(above($1,135,$13))   w st lt 3 lw 1 t  '',\
    ''  u 1:(below($1,115,$16))   w st lt -1 lw 2 t @pilesub,\
    ''  u 1:(above($1,135,$17))   w st lt -1 lw 2 t  ''"

nopile="'no pileup'"
nopilesub="'no pileup, sub'"
pile="''"
pilesub="''"

set size 1,1.1
set multiplot layout 3,1 scale 1,1.3

set x2tics
set format x " "
set xlabel " "

set label 1  'k_t, R=0.4'   at graph 0.035,0.91 


plot kt @plotobj

#--- cam
nopile="''"
nopilesub="''"
pile="'pileup'"
pilesub="'pileup, sub'"
set label 1  'Cam/Aachen, R=0.4' 

#unset key
set format x2 " "
plot cam @plotobj

#--- last one (cone)
set label 2 'LHC, high lumi' at graph 0.965,0.91 right
unset key
set format x "%g"
set xlabel 'reconstructed W / top mass [GeV]'
set label 1  'SISCone, R=0.4, f=0.5'
plot cone @plotobj


unset multiplot
set output 
`ps2bbx @filename`
set term @GNUTERM


#`gnupr W+t-mass-kt.eps col`

# plot 'highlumi_cam_r0.4' \
#         u 1:(below($1,115,$4))    w st lt 2 lw 2 t  'cam, no pileup',\
#     ''  u 1:(above($1,135,$5))    w st lt 2 lw 2 t  '',\
#     ''  u 1:(below($1,115,$12))   w st lt 3 lw 2 t  'cam, pileup',\
#     ''  u 1:(above($1,135,$13))   w st lt 3 lw 2 t  '',\
#     ''  u 1:(below($1,115,$16))   w st lt -1 lw 3 t  'cam, pileup, corrected',\
#     ''  u 1:(above($1,135,$17))   w st lt -1 lw 3 t  ''
# `gnupr W+t-mass-cam.eps col`
