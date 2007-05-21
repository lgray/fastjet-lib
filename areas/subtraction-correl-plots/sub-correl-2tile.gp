# gnuplot file

reset
unset multiplot

set macros
set fit errorvariables

set term postscript portrait enhanced color size 25cm,10cm colortext 
filename="sub-correl-2tile.eps"
set output filename

algA='kt-R0.7-ptmin'
algB='cam-R0.7-ptmin'
algC='siscone-R0.7-f0.5-ptmin'
#algB='siscone-R0.7-f0.75-ptmin'

set xlabel 'p_{t,jet} [GeV]' offset 0,1




lstyA="lt 1"; styA="@lstyA pt 7 ps 0.4"
lstyB="lt 2"; styB="@lstyB pt 1 ps 0.4"
lstyC="lt 3"; styC="@lstyC pt 3 ps 0.4"

rawp='u 1:($7-$1) w p'
subp='u 2:($8-$2) w p'
sube='u 2:($8-$2):9 w e'

set log x
xmin=60
set xrange [xmin:]
set yrange [-20:60]

fit [xmin:] meanA "< cat ".algA."*.res" u 1:($7-$1) via meanA
fit [xmin:] meanB "< cat ".algB."*.res" u 1:($7-$1) via meanB
fit [xmin:] meanC "< cat ".algC."*.res" u 1:($7-$1) via meanC

fit [xmin:] mnsbA "< cat ".algA."*.res" u 2:($8-$2):9 via mnsbA
fit [xmin:] mnsbB "< cat ".algB."*.res" u 2:($8-$2):9 via mnsbB
fit [xmin:] mnsbC "< cat ".algC."*.res" u 2:($8-$2):9 via mnsbC

set zeroaxis

set multiplot layout 1,2

set label 1 'a) raw' at graph 0.05,0.95
set label 2 'LHC, high lumi.' at graph 0.97,0.05 right
set ylabel 'p_{t,jet+PU} - p_{t,jet} [GeV]'
plot "< cat ".algA."*.res"  @rawp @styA t '',\
     "< cat ".algB."*.res"  @rawp @styB t '',\
     "< cat ".algC."*.res"  @rawp @styC t '',\
    meanA w l @lstyA lw 2 t '',\
    meanB w l @lstyB lw 2 t '',\
    meanC w l @lstyC lw 2 t ''

unset label 2

set key spacing 1.3 samplen 1 box
set label 1 'b) subtracted'
set ylabel 'p_{t,jet+PU,sub} - p_{t,jet,sub} [GeV]'
set xlabel 'p_{t,jet,sub} [GeV]'
plot "< cat ".algA."*.res"  @subp @styA t 'k_t',\
     "< cat ".algB."*.res"  @subp @styB t 'Cam/Aachen',\
     "< cat ".algC."*.res"  @subp @styC t 'SISCone',\
    mnsbA w l @lstyA lw 2 t '',\
    mnsbB w l @lstyB lw 2 t '',\
    mnsbC w l @lstyC lw 2 t ''

unset multiplot

set output
set term @GNUTERM

`ps2bbx @filename`

print "A = ",meanA, "   ",mnsbA," +- ",mnsbA_err
print "B = ",meanB, "   ",mnsbB," +- ",mnsbB_err
print "C = ",meanC, "   ",mnsbC," +- ",mnsbC_err


