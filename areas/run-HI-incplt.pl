#!/usr/bin/perl -w

$iseqlo=0;
$nseq=10;

#$r=0.4;
$r=0.25;

$cam=1;
#$cam=0;


# set info related to choice of cambridge...
$camname="";
$camopt="";
if ($cam eq 1) {
  $camname="_cam";
  $camopt="-cam";
} else {
}

for ($iseq=$iseqlo; $iseq < $iseqlo+$nseq; $iseq++) {
  $iseqformat = sprintf("%03d",$iseq);
  #$command = "submitjob.pl BEST ./run-mass-test.pl -nev 50000 -out hydjet_inclpt/incplt_nhsel2_r".$r."_iseq$iseqformat -r $r -nhsel 2 -ptmin 10 -ptminhard 10 -ptmaxhard -1 -freq 100 -iseq $iseq";

  # $command = "submitjob.pl BEST ./run-mass-test.pl -nev 50000 -out hydjet_inclpt/incplt_ptminhard50_nhsel2_r".$r."_iseq$iseqformat -r $r -nhsel 2 -ptmin 10 -ptminhard 50 -ptmaxhard -1 -freq 100 -iseq $iseq";
  # 
  # #print $command."\n";
  # system("$command");

  ### TEMPORARY -- THIS ONE HAS A DIFFERENT NAME BECAUSE WE HAD JUST 
  ### INTRODUCED A NEW OUTPUT FORMAT
  #$command = "submitjob.pl BEST ./run-mass-test.pl -nev 50000 -out hydjet_inclpt/incplt_xcl_ptminhard50_nhsel2".$camname."_r".$r."_iseq$iseqformat $camopt -r $r -nhsel 2 -ptmin 10 -ptminhard 50 -ptmaxhard -1 -freq 100 -iseq $iseq";

  $command = "submitjob.pl BEST ./run-mass-test.pl -nev 100000 -out hydjet_inclpt/incplt_xcl_nhsel1".$camname."_r".$r."_iseq$iseqformat $camopt -r $r -nhsel 1 -ptmin 10 -ptminhard 10 -ptmaxhard -1 -freq 100 -iseq $iseq -strategy -4";
  
  #print $command."\n";
  system("$command");
  
  $command = "submitjob.pl BEST ./run-mass-test.pl -nev 100000 -out hydjet_inclpt/incplt_xcl_ptminhard50_nhsel1".$camname."_r".$r."_iseq$iseqformat $camopt -r $r -nhsel 1 -ptmin 10 -ptminhard 50 -ptmaxhard -1 -freq 100 -iseq $iseq  -strategy -4";
  
  #print $command."\n";
  system("$command");


  for ($nhsel=3; $nhsel<=3; $nhsel++) {
  #for ($nhsel=3; $nhsel<=4; $nhsel++) {
  #for ($nhsel=4; $nhsel<=4; $nhsel++) {
    # getting just hard jets...
    $command = "submitjob.pl BEST ./run-mass-test.pl -nev 5000000 -out hydjet_inclpt/incplt_justhard_ptminhard50_nhsel".$nhsel.$camname."_r".$r."_iseq$iseqformat $camopt -r $r -nhsel $nhsel -ptmin 300 -ptminhard 50 -ptmaxhard -1 -freq 1000 -iseq $iseq  -cell_area 1.0";
    #print $command."\n";
    system("$command");
    
    $command = "submitjob.pl BEST ./run-mass-test.pl -nev 5000000 -out hydjet_inclpt/incplt_justhard_nhsel".$nhsel.$camname."_r".$r."_iseq$iseqformat $camopt -r $r -nhsel $nhsel -ptmin 300 -ptminhard 10 -ptmaxhard -1 -freq 1000 -iseq $iseq -cell_area 1.0";
    #print $command."\n";
    system("$command");
  }
}
