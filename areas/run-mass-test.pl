#!/usr/bin/perl -w
#
# script for running pythia and piping its output into some jet-finding
# program (splits options up appropriately between the two programs)

# determine user running job
$user=`whoami`; chomp $user;
if ($user eq 'salam') {$basedir = '/ada1/lpthe/salam/work/fastjet';}
elsif ($user eq 'cacciari') {$basedir = '/ada1/lpthe/cacciari/physics/voronoi/fastjet';}
else {print 'Unkown user\n'; exit;}

# determine architecture
# Piping does not work on Mac. Don't use it if running there
$arch=`uname`; chomp $arch;
if ($arch eq 'Darwin') { $pipe = 0; } else { $pipe = 1;}

# if cacciari is running on linux, use Gavin's executables
if ($user eq 'cacciari' && $arch eq 'Linux') 
                 {$basedir = '/ada1/lpthe/salam/work/fastjet';}

# Executables
$pythia_exec = "$basedir/pythia/gen-events";
$jet_exec    = "$basedir/fastjet-release/areas/subtraction-tests-mass";

#$hydjet_exec = "$basedir/hydjet/test2_hydjet";
$hydjet_exec = "$basedir/hydjet/run_hydjet";
$incljet_exec = "$basedir/fastjet-release/areas/subtraction-tests-inclpt";
$ttbarjet_exec = "$basedir/fastjet-release/areas/subtraction-tests-ttbar";
$HIeff_exec = "$basedir/fastjet-release/areas/subtraction-tests-HIeff";
$HIeff_iter_exec = "/ada1/lpthe/cacciari/physics/voronoi/fastjet/fastjet-release/areas/subtraction-HIeff-iter";

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
  elsif ($opt eq '-nh' || $opt eq '-ytfl' || $opt eq '-ylfl')  {$pythia_opts .= " $opt ".(shift @ARGV); $run_hydjet=1;}
  elsif ($opt eq '-CMS')    {$pythia_opts .= " $opt";}
  elsif ($opt eq '-rhic')    {$pythia_opts .= " $opt";}
  elsif ($opt eq '-ptminhard')  {$pythia_opts .= " $opt ".(shift @ARGV);}
  elsif ($opt eq '-ptmaxhard')  {$pythia_opts .= " $opt ".(shift @ARGV);}
  # remaining opts go to jet prog
  elsif ($opt eq '-HIeff')  {$run_hydjet=1; $run_HIeff = 1;}
  elsif ($opt eq '-HIeff_iter')  {$run_hydjet=1; $run_HIeff_iter = 1;}
  else  {$jet_opts .= " $opt";}
}
$pythia_opts .= " -nev $nev -out $pipename";
$jet_opts    .= " -nev $nev -in  $pipename -out $outfile";

# make the pipe that will be used for communication between the
# programs
if ($pipe) {system("mknod $pipename p");}

# replace executables in this case to look at incl-pt spectrum with hydjet...
if ($run_hydjet) {
  $pythia_exec = $hydjet_exec;
  if ($run_HIeff) {
    $jet_exec = $HIeff_exec;
  } elsif ($run_HIeff_iter ) {
    $jet_exec = $HIeff_iter_exec;
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
if (not $pipe) {sleep(20);}
# for the original program
system($analysis); 

if ($pipe) {
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
}


#system("$pythia &");
#system("$analysis");


# clean up...
if ($pipe) {unlink($pipename);}
else {system("rm $pipename");}

