# gnuplot file

reset
unset multiplot

set macros
set fit errorvariables

set term postscript portrait enhanced color size 33cm,9cm colortext 
set grid noxtics noytics front
filename="sub-correl-2tile.eps"
#filename="a.eps"
set output filename

algA='kt-R0.7-ptmin'
algB='cam-R0.7-ptmin'
#algC='siscone-R0.7-f0.5-ptmin'
algC='siscone-R0.7-f0.5-passive-ptmin'
#algB='siscone-R0.7-f0.75-ptmin'

filelist="*-RUK.res"

set xlabel 'p_{t,jet} [GeV]' offset 0,1



lstyA="lt 1"; styA="@lstyA pt 7 ps 0.4"
lstyB="lt 2"; styB="@lstyB pt 1 ps 0.4"
lstyC="lt 3"; styC="@lstyC pt 3 ps 0.4"

rawp='u 1:($7-$1) w p'
subp='u 2:($8-$2) w p'
sube='u 2:($8-$2):(sqrt($9**2+$3**2)) w e'

set log x
xmin=60
xmax=1000
set xrange [xmin:xmax]
set yrange [-20:60]

xmnft=xmin
xmxft=xmax
#xmnft=100
#xmxft=1000
#fit [xmnft:xmxft] meanA+AA*log(x/200) "< cat ".algA.filelist u 1:($7-$1) via meanA,AA
#fit [xmnft:xmxft] meanB+BB*log(x/200) "< cat ".algB.filelist u 1:($7-$1) via meanB,BB
#fit [xmnft:xmxft] meanC+CC*log(x/200) "< cat ".algC.filelist u 1:($7-$1) via meanC,CC
AA=0; BB=0; CC=0
fit [xmnft:xmxft] meanA "< cat ".algA.filelist u 1:($7-$1) via meanA
fit [xmnft:xmxft] meanB "< cat ".algB.filelist u 1:($7-$1) via meanB
fit [xmnft:xmxft] meanC "< cat ".algC.filelist u 1:($7-$1) via meanC

#fit [xmnft:xmxft] mnsbA+sA*log(x/200) "< cat ".algA.filelist u 2:($8-$2):9 via mnsbA,sA
#fit [xmnft:xmxft] mnsbB+sB*log(x/200) "< cat ".algB.filelist u 2:($8-$2):9 via mnsbB,sB
#fit [xmnft:xmxft] mnsbC+sC*log(x/200) "< cat ".algC.filelist u 2:($8-$2):9 via mnsbC,sC
sA=0; sB=0; sC=0
fit [xmnft:xmxft] mnsbA "< cat ".algA.filelist u 2:($8-$2):9 via mnsbA
fit [xmnft:xmxft] mnsbB "< cat ".algB.filelist u 2:($8-$2):9 via mnsbB
fit [xmnft:xmxft] mnsbC "< cat ".algC.filelist u 2:($8-$2):9 via mnsbC

set zeroaxis

set multiplot layout 1,2

set label 1 'a) raw' at graph 0.05,0.95
set label 2 'LHC, high lumi.' at graph 0.97,0.05 right
set ylabel 'p_{t,jet+PU} - p_{t,jet} [GeV]'
plot "< cat ".algA.filelist  @rawp @styA t '',\
     "< cat ".algB.filelist  @rawp @styB t '',\
     "< cat ".algC.filelist  @rawp @styC t '',\
    meanA+AA*log(x/200) w l @lstyA lw 2 t '',\
    meanB+BB*log(x/200) w l @lstyB lw 2 t '',\
    meanC+CC*log(x/200) w l @lstyC lw 2 t ''

#    meanA+AA*log(x/200) w l @lstyA lw 2 t '',\
#    meanB w l @lstyB lw 2 t '',\
#    meanC w l @lstyC lw 2 t ''

unset label 2

set key spacing 1.3 samplen 1 box
set label 1 'b) subtracted'
set ylabel 'p_{t,jet+PU,sub} - p_{t,jet,sub} [GeV]'
set xlabel 'p_{t,jet,sub} [GeV]'
plot "< cat ".algA.filelist  @sube @styA t 'k_t',\
     "< cat ".algB.filelist  @sube @styB t 'Cam/Aachen',\
     "< cat ".algC.filelist  @sube @styC t 'SISCone',\
    mnsbA+sA*log(x/200) w l @lstyA lw 2 t '',\
    mnsbB+sB*log(x/200) w l @lstyB lw 2 t '',\
    mnsbC+sC*log(x/200) w l @lstyC lw 2 t ''


#    mnsbA w l @lstyA lw 2 t '',\
#    mnsbB w l @lstyB lw 2 t '',\
#    mnsbC w l @lstyC lw 2 t ''

unset multiplot

set output
set term @GNUTERM

`ps2bbx @filename`

print "A = ",meanA," +- ",meanA_err, "   ",mnsbA," +- ",mnsbA_err
print "B = ",meanB," +- ",meanB_err, "   ",mnsbB," +- ",mnsbB_err
print "C = ",meanC," +- ",meanC_err, "   ",mnsbC," +- ",mnsbC_err


