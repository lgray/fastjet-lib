# gnuplot file for setting up a bunch of variables
# and macros related to jet plots
#reset

set macros

# each of these functions returns a string corresponding to
# jet alg, R and m=0|1 -> noUE(MI)|withUE(MI)
kt(R,m)     = sprintf("kt_%3.1f_RUK%s.res",R,(m==1)?"":"_noMI")
cam(R,m)    = sprintf("cam_%3.1f_RUK%s.res",R,(m==1)?"":"_noMI")
antikt(R,m) = sprintf("antikt_%3.1f_RUK%s.res",R,(m==1)?"":"_noMI")
siscone(R,m)= sprintf("siscone_%3.1f_f0.75_RUK%s.res",R,(m==1)?"":"_noMI")
midpoint(R,m) = sprintf("midpoint_%3.1f_f0.75_%s.res",R,(m==1)?"voronoi_RUK":"RUK_noMI")
midpoint05(R,m) = sprintf("midpoint_seed0.5_%3.1f_f0.75_%s.res",R,(m==1)?"voronoi_RUK":"RUK_noMI")
midpoint20(R,m) = sprintf("midpoint_seed2.0_%3.1f_f0.75_%s.res",R,(m==1)?"voronoi_RUK":"RUK_noMI")
jetclu(R,m) = sprintf("jetclu_%3.1f_f0.75_%s.res",R,(m==1)?"voronoi_RUK":"RUK_noMI")
jetclu01(R,m) = sprintf("jetclu_seed0.1_%3.1f_f0.75_%s.res",R,(m==1)?"voronoi_RUK":"RUK_noMI")
jetclu05(R,m) = sprintf("jetclu_seed0.5_%3.1f_f0.75_%s.res",R,(m==1)?"voronoi_RUK":"RUK_noMI")
jetclu20(R,m) = sprintf("jetclu_seed2.0_%3.1f_f0.75_%s.res",R,(m==1)?"voronoi_RUK":"RUK_noMI")


# get a whole plot entry corresponding to the algorithm,R,m, column
# line type and line width
entry(alg,R,m,col,lt,lw)=sprintf("%s(%f,%d) u 2:%d w l lt %d lw %d",\
                         alg,R,m,col,lt,lw)

mW_mt=80.45/175.0

entryW(alg,R,m,col,lt,lw)=sprintf("%s(%f,%d) u ($2/mW_mt):($%d*mW_mt) w l lt %d lw %d", alg,R,m,col,lt,lw)

# get a series of different R values
series(alg,m,col,lt)=entry(alg,0.4,m,col,lt,1)."t '".alg." R = 0.4'".",".\
                     entry(alg,0.5,m,col,lt,2)."t '".alg." R = 0.5'".",".\
                     entry(alg,0.6,m,col,lt,3)."t '".alg." R = 0.6'".",".\
                     entry(alg,0.8,m,col,lt,4)."t '".alg." R = 0.8'".",".\
                     entry(alg,1.0,m,col,lt,5)."t '".alg." R = 1.0'"

# get a series of different algs
algs(R,m,col,lw) =   entry("kt",      R,m,col,1,lw)."t 'k_t',".\
                     entry("cam",     R,m,col,2,lw)."t 'Cam/Aachen',".\
                     entry("antikt",  R,m,col,3,lw)."t 'anti-k_t',".\
                     entry("jetclu",  R,m,col,4,lw)."t 'jetclu',".\
                     entry("siscone", R,m,col,5,lw)."t 'siscone'"

# get a series of different algs with jetclu rescaled 
algsresc(R,m,col,lw) =   entry("kt",      R,m,col,1,lw)."t 'k_t',".\
                     entry("cam",     R,m,col,2,lw)."t 'Cam/Aachen',".\
                     entry("antikt",  R,m,col,3,lw)."t 'anti-k_t',".\
                     entry("jetclu", 0.8*R,m,col,4,lw)."t 'jetclu (R*0.8)',".\
                     entry("siscone", R,m,col,5,lw)."t 'siscone'"


TWresc(alg,R,m,col,lw) = entry(alg,R,m,col,1,lw)."t 'top',".\
                         entryW(alg,R,m,col-1,3,lw)."t 'W rescaled'"
