# gnuplot file

reset
set st dat li

xmin=1900
xmax=2150
set xrange  [xmin:xmax]
set x2range [xmin:xmax]

unset multiplot
filename="Zpmass.eps"
#filename="Wt.eps"
set term postscript enhance color dashed portrait size 15cm,20cm
set output filename

set ylabel '1/N dN/dm [GeV^{-1}]' offset 2,0

set key spacing 1.5

binsize=2
binshift=binsize*0.5


set macros

R="0.7"
#alg='tmp/cam-r1.0-iseq001-pileup-RUK.res'
alg='tmp/kt-r'.R.'-iseq001-pileup-RUK.res'
#alg='cam-r1-iseq001-pileup-shiftedhist.res'
#alg='kt-r1-iseq001-pileup.res'
#alg='siscone-r1-f0.5-passive-RUK-iseq001-pileup.res'

linenp ="lt  2 lw 2 lc rgb '#00a000'"
linenps="lt  1 lw 3"
linepu ="lt  3 lw 2"
linepus="lt -1 lw 2"

plotobj="\
        u 1:($2)    w histeps @linenp  t  @nopile,\
    ''  u 1:($6)    w histeps @linenps t  @nopilesub,\
    ''  u 1:($4)    w histeps @linepu  t  @pile,\
    ''  u 1:($7)    w histeps @linepus t @pilesub"

nopile="'no pileup'"
nopilesub="'no pileup, sub'"
pile="'pileup'"
pilesub="'pileup, sub'"

set size 1,1.1
set multiplot layout 3,1 scale 1,1.3

set label 1  'k_t, R='.R   at graph 0.035,0.91 
set label 2  "LHC, high lumi"   at graph 0.035,0.79 
set label 3  "Z{/Symbol \242} at 2 TeV"   at graph 0.035,0.70


#--- cam
#set label 1  'Cam/Aachen, R=1' 
#set yrange [0:0.014]
set yrange [0:0.015]
set ytics 0.005
set mytics 5
set xtics 100
set mxtics 10
set xlabel 'm [GeV]'

#unset key
set format x2 " "
plot alg @plotobj



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
