# gnuplot file


reset
set sty dat li

set yrange [0.23:0.35]
set xrange [0:2.0]

set xlabel '{/Symbol D}_{12}/R'
set ylabel 'A_{cone,R}({/Symbol D}_{12}) / {/Symbol p}R^2' offset 1

#set label 1 'thin lines: passive area' at 0.04,1.7
#set label 2 'thick lines: active area' at 1.9,0.5 right

set ytics 0.05
set mytics 5

set size 1.0,0.6

#set grid

set key spacing 1.5

# raw kt area according to CCN27-76
fktraw(d) = d*sqrt(1-d**2/4.0) + 2*(pi - acos(d/2))

# it gets converted into a deficit by taking the effective kt area formula
# and dividing by two
fdeficit(d) = (d<2&&d>=1) ? 0.5*fktraw(d) : pi

#  kt area according to CCN27-76
fkt(d) = (d<1) ?  fktraw(d) : fdeficit(d)

# cam area according to CCN27-79
fcam(d) = (d<1 && d>0.5) ? \
        pi-acos(1.0/(2*d))+sqrt(d**2-0.25)+d**2*acos(1/(2*d**2)-1) \
        : fdeficit(d)

# cone passive area
fcone(d) = (d<1) ? pi : fdeficit(d)

# cone active area (DIVIDED BY PI, UNLIKE THE OTHERS)
fconeactive1(d) = 1.0/4.0*(1.0 - 1.0/pi*acos(d/2.0)) + \
                  d/2.0/pi*sqrt(1.0-d**2/4.0) + \
                  0.25/pi*sqrt(1.0-d**2)*acos(d/(2.0-d**2))

fconeactive2(d) = 1.0/4.0*(1.0 - 1.0/pi*acos(d/2.0)) + \
                  d/2.0/pi*sqrt(1.0-d**2/4.0) - \
                  0.125*d/pi/sqrt(1.0 - d**2/4.0)

fconeactive3(d) = 1.0/4.0*(1.0 - 1.0/pi*acos(d/2.0)) + \
                  1.0/2.0/pi/d*sqrt(1.0-d**2/4.0)

fconeactive(d) = (d<1) ? fconeactive1(d) : ((d<sqrt(2.0)) ? fconeactive2(d) : fconeactive3(d))


ktpassive = 1; set style line ktpassive lt 1 lw 1 
campassive = 2; set style line campassive lt 2 lw 1 
conepassive = 3; set style line conepassive lt 3 lw 1 
ktactive = 4; set style line ktactive lt 1 lw 6
camactive = 5; set style line camactive lt 2 lw 6
coneactive = 6; set style line coneactive lt 3 lw 6 


set sample 1000

plot 0.25 w l lt 0 t '',\
     fconeactive(x) w l ls coneactive t ''           
