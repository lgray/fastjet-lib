#!/usr/bin/perl -w
#
# script for running pythia and piping its output into some jet-finding
# program (splits options up appropriately between the two programs)

$pythia_exec = "../../pythia/gen-events";
$jet_exec    = "./subtraction-tests-mass";

#$hydjet_exec = "../../hydjet/test2_hydjet";
$hydjet_exec = "../../hydjet/run_hydjet";
$incljet_exec = "./subtraction-tests-inclpt";
$ttbarjet_exec = "./subtraction-tests-ttbar";
$HIeff_exec = "./subtraction-tests-HIeff";

# establish a hopefully unique name for named-pipe
$hostname=`hostname -s`; chomp $hostname;
$pipename="/tmp/.pipe-$hostname-$$";


$this_prog   = $0." ".join(" ",@ARGV);
$pythia_opts = "";
$jet_opts    = "";
$nev         = 0;
$outfile     = "";
$run_hydjet  = 0;
$run_HIeff   = 0;

# extract the options that will go to pythia / jet-prof
while ($#ARGV >= 0) {
  $opt = shift @ARGV;
  # opts that need special treatment
  if    ($opt eq '-nev')    {$nev = shift @ARGV;}
  if    ($opt eq '-out')    {$outfile = shift @ARGV;}
  # pythia opts (logical)
  elsif ($opt eq '-Z2jets') {$pythia_opts .= " $opt";}
  elsif ($opt eq '-Zp2jets'){$pythia_opts .= " $opt";}
  elsif ($opt eq '-ttbar')  {$pythia_opts .= " $opt"; $jet_exec = $ttbarjet_exec;}
  elsif ($opt eq '-lolumi') {$pythia_opts .= " $opt";}
  elsif ($opt eq '-noMI')   {$pythia_opts .= " $opt";}
  elsif ($opt eq '-pileup') {$pythia_opts .= " $opt";}
  elsif ($opt eq '-lhc')    {$pythia_opts .= " $opt";}
  elsif ($opt eq '-tev')    {$pythia_opts .= " $opt";}
  elsif ($opt =~ /^-minbias/){$pythia_opts .= " $opt";}
  # pythia opts (others)
  elsif ($opt eq '-lumi')   {$pythia_opts .= " $opt ".(shift @ARGV);}
  elsif ($opt eq '-ptmin')  {$pythia_opts .= " $opt ".(shift @ARGV);}
  elsif ($opt eq '-Zpmass') {$pythia_opts .= " $opt ".(shift @ARGV);}  elsif ($opt eq '-Zpmass') {$pythia_opts .= " $opt ".(shift @ARGV);}
  elsif ($opt eq '-iseq')   {$pythia_opts .= " $opt ".(shift @ARGV);}
  # hydjet options...
  elsif ($opt eq '-nhsel')  {$pythia_opts .= " $opt ".(shift @ARGV); $run_hydjet=1;}
  elsif ($opt eq '-HIeff')  {$run_hydjet=1; $run_HIeff = 1;}
  elsif ($opt eq '-ptminhard')  {$pythia_opts .= " $opt ".(shift @ARGV);}
  elsif ($opt eq '-ptmaxhard')  {$pythia_opts .= " $opt ".(shift @ARGV);}
  # remaining opts go to jet prog
  else  {$jet_opts .= " $opt";}
}
$pythia_opts .= " -nev $nev -out $pipename";
$jet_opts    .= " -nev $nev -in  $pipename -out $outfile";

# make the pipe that will be used for communication between the
# programs
system("mknod $pipename p");

# replace executables in this case to look at incl-pt spectrum with hydjet...
if ($run_hydjet) {
  $pythia_exec = $hydjet_exec;
  if ($run_HIeff) {
    $jet_exec = $HIeff_exec;
  } else {
    $jet_exec    = $incljet_exec;
  }
}

# run pythia and the analysis program separately
$pythia   = "$pythia_exec $pythia_opts";
$analysis = "$jet_exec $jet_opts -rerun '$this_prog'";
print STDERR $pythia."\n";
print STDERR $analysis."\n";
# need to replace the following combination with something like
$pid = fork();
# for the slave...
print "NEW PID IS $pid\n";
if ($pid == 0) {system("$pythia"); exit(0);}
# for the original program
system($analysis); 

# since processes writing to pipes can lead to large temporary files
# (and since it's not nice to leave dead processes around), do our
# best to kill the slave and its children, if necessary...
$slave_alive=(kill 0, $pid);
if ($slave_alive) {
  $slave_details=`ps -fp $pid | grep -v PID`;
  if ($slave_details !~ /defunct/) {
    print "======== generator ($pid+children) is still running and will be killed:\n";
    $procs=`ps --ppid $pid | grep -v PID | awk '{print \$1}'`;
    @procs=split("\n",$procs);
    push @procs, $pid;
    foreach $proc (@procs) {
      system("ps -fp $proc | grep -v PID");
      system("kill -9 $proc");
    }
  }
}


#system("$pythia &");
#system("$analysis");


# clean up...
unlink($pipename);

