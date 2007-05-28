# gnuplot file
reset

set macros

jetalg(R)=sprintf("kt_%3.1f_RUK_noMI.res",R)
series="jetalg(0.4) u 2:5 w l lt 1, jetalg(0.5) u 2:5 w l lt 1 lw 2, jetalg(0.6) u 2:5 w l lt 1 lw 3, jetalg(0.8) u 2:5 w l lt 1 lw 4, jetalg(1.0) u 2:5 w l lt 1 lw 5"

jetalg2(R)=sprintf("jetclu_%3.1f_RUK_noMI.res",R)
series2="jetalg2(0.4) u 2:5 w l lt 1, jetalg2(0.5) u 2:5 w l lt 1 lw 2, jetalg2(0.6) u 2:5 w l lt 1 lw 3, jetalg2(0.8) u 2:5 w l lt 1 lw 4, jetalg2(1.0) u 2:5 w l lt 1 lw 5"


seriesA(col)=sprintf("jetalg(0.4) u 2:%d w l lt 1, jetalg(0.%d) u 2:%d w l lt 1 lw 2, jetalg(0.6) u 2:%d w l lt 1 lw 3, jetalg(0.8) u 2:%d w l lt 1 lw 4, jetalg(1.0) u 2:%d w l lt 1 lw 5",col,col,col,col,col)