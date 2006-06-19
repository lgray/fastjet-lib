#!/bin/zsh


# some cambridge/kt runs...
nev=1e5; freq=500

#opts=(-Zp2jets -Zpmass 2000 -max 4000 -bin 10.0)
#optname="zprime-"

opts=(-Z2jets -max 400 -bin 2.0)
optname=""

for r in 0.4 0.7 1.0
#for r in 0.7
do
      # cam jobs (no area correction)
      #submitjob.pl BEST ./run-mass-test.pl -cell_area 1.0 -nev $nev -freq $freq -r $r -lhc -pileup $opts  -cam -out mass-tests/${optname}cam-highlumi-r${r}ext.res
      #submitjob.pl BEST ./run-mass-test.pl -cell_area 1.0 -nev $nev -freq $freq -r $r -lhc -noMI $opts  -cam -out mass-tests/${optname}cam-zerolumi-noMI-r${r}ext.res
      #submitjob.pl BEST ./run-mass-test.pl -cell_area 1.0 -nev $nev -freq $freq -r $r -lhc -pileup -lolumi $opts -cam -out mass-tests/${optname}cam-lowlumi-r${r}ext.res


     # cam jobs (with area correction)
     submitjob.pl BEST ./run-mass-test.pl -cam -nev $nev -freq $freq -r $r -lhc -pileup $opts   -out mass-tests/${optname}camarea-highlumi-r${r}ext.res
     submitjob.pl BEST ./run-mass-test.pl -cam -nev $nev -freq $freq -r $r -lhc -noMI $opts   -out mass-tests/${optname}camarea-zerolumi-noMI-r${r}ext.res
     submitjob.pl BEST ./run-mass-test.pl -cam -nev $nev -freq $freq -r $r -lhc -pileup -lolumi $opts -out mass-tests/${optname}camarea-lowlumi-r${r}ext.res

     # kt jobs (with area correction)
     submitjob.pl BEST ./run-mass-test.pl -nev $nev -freq $freq -r $r -lhc -pileup $opts   -out mass-tests/${optname}highlumi-r${r}ext.res
     submitjob.pl BEST ./run-mass-test.pl -nev $nev -freq $freq -r $r -lhc -noMI $opts   -out mass-tests/${optname}zerolumi-noMI-r${r}ext.res
     submitjob.pl BEST ./run-mass-test.pl -nev $nev -freq $freq -r $r -lhc -pileup -lolumi $opts -out mass-tests/${optname}lowlumi-r${r}ext.res
done

# # some cone runs
# nev=5e3; freq=40
# for r in 0.7
# for iseq in 01 02 03 04 05 06 07 08 09 10
# do
#     #submitjob.pl BEST ./run-mass-test.pl -cone -nev $nev -freq $freq -r $r -lhc -pileup $opts -iseq $iseq -out mass-tests/${optname}cone-highlumi-r$r-iseq$iseq.res
#     submitjob.pl BEST ./run-mass-test.pl -cone075 -nev $nev -freq $freq -r $r -lhc -pileup $opts -iseq $iseq -out mass-tests/${optname}cone075-highlumi-r$r-iseq$iseq.res
#     #submitjob.pl BEST ./run-mass-test.pl -searchcone -nev $nev -freq $freq -r $r -lhc -pileup $opts -iseq $iseq -out mass-tests/${optname}searchcone-highlumi-r$r-iseq$iseq.res
# done
