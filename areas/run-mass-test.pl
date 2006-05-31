#!/usr/bin/perl -w
#
# script for running pythia and piping its output into some jet-finding
# program (splits options up appropriately between the two programs)

$pythia_exec = "../../pythia/gen-events";
$jet_exec    = "./subtraction-tests-mass";

# establish a hopefully unique name for named-pipe
$hostname=`hostname -s`; chomp $hostname;
$pipename="/tmp/.pipe-$hostname-$$";


$this_prog   = $0." ".join(" ",@ARGV);
$pythia_opts = "";
$jet_opts    = "";
$nev         = 0;
$outfile     = "";

# extract the options that will go to pythia / jet-prof
while ($#ARGV >= 0) {
  $opt = shift @ARGV;
  # opts that need special treatment
  if    ($opt eq '-nev')    {$nev = shift @ARGV;}
  if    ($opt eq '-out')    {$outfile = shift @ARGV;}
  # pythia opts (logical)
  elsif ($opt eq '-Z2jets') {$pythia_opts .= " $opt";}
  elsif ($opt eq '-Zp2jets'){$pythia_opts .= " $opt";}
  elsif ($opt eq '-lolumi') {$pythia_opts .= " $opt";}
  elsif ($opt eq '-noMI')   {$pythia_opts .= " $opt";}
  elsif ($opt eq '-pileup') {$pythia_opts .= " $opt";}
  elsif ($opt eq '-lhc')    {$pythia_opts .= " $opt";}
  elsif ($opt eq '-tev')    {$pythia_opts .= " $opt";}
  elsif ($opt =~ /^-minbias/){$pythia_opts .= " $opt";}
  # pythia opts (others)
  elsif ($opt eq '-ptmin')  {$pythia_opts .= " $opt ".(shift @ARGV);}
  elsif ($opt eq '-Zpmass') {$pythia_opts .= " $opt ".(shift @ARGV);}
  elsif ($opt eq '-iseq')   {$pythia_opts .= " $opt ".(shift @ARGV);}
  # remaining opts go to jet prog
  else  {$jet_opts .= " $opt";}
}
$pythia_opts .= " -nev $nev -out $pipename";
$jet_opts    .= " -nev $nev -in  $pipename -out $outfile";

# make the pipe that will be used for communication between the
# programs
system("mknod $pipename p");

# run pythia and the analysis program separately
$pythia   = "$pythia_exec $pythia_opts";
$analysis = "$jet_exec $jet_opts -rerun '$this_prog'";
print STDERR $pythia."\n";
print STDERR $analysis."\n";
system("$pythia &");
system("$analysis");

#$command = "$pythia_exec $pythia_opts | $jet_exec $jet_opts -rerun '$this_prog'";
#system($command);

# 
unlink($pipename);

# # now add some information to beginning of outfile
# $outres  = "# $this_prog\n";
# $outres .= "# $command\n";
# 
# open (OUT, ">$outfile") || die "Failed to open $outfile for rewriting";
# print OUT $outres;
# 
# open(IN, "<$outfile.tmp") || die "Failed to open $outfile.tmp for reading";
# while ($line = <IN>) {print OUT $line}
# close(IN);
# close OUT;
# system("rm $outfile.tmp");
