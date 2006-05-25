#!/bin/bash

#-- main part of configuration
nev=1000
inputfile=~/work/fastjet/data/Pythia-PtMin50-LHC-1000ev.dat
command="./fastjet_timing -write -nev $nev"
#---------------------------

tmpbase=tmp-$$

# create a reference file
strategy=-3
echo Reference strategy is $strategy, number of events is $nev
reffile=$tmpbase-$strategy
$command -strategy $strategy < $inputfile | grep -v strategy > $reffile


for strategy in -2 -1 +2 +3 +4 +0
do
  echo -n "Strategy $strategy ... "
  thisfile=$tmpbase-$strategy
  $command -strategy $strategy < $inputfile  | grep -v strategy > $thisfile
  echo -n "Number of differences is: "
  diff $thisfile $reffile | wc -l
  rm $thisfile
done

rm $reffile
