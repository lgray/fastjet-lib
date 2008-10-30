#!/usr/bin/perl -w
#
# This script is intended to become a part of a testsuite for all the
# clustering algorithms in FastJet.
#
# It works by running example/fasjet_timing_plugins for a jet
# definition, extracting either the jets (cone algs) or the
# written-out sequence (seq.rec. algs) and then taking the md5sum
# of the results.
#
# It contains a bunch of stored md5sums for various cases, against
# which it carries out a quick test.
#
#
# Various command-line options are available:
#
#  -nev NEV          sets the number of events to use (default = ???)
#
#  -alg ALGNAME      sets the alg name and all parameters other than R
#
#  -R   R            sets R
#
#  -strat STRAT      use only this strategy [1 == best]
#
#  -deposit DIR      puts the (unfiltered) output in a directory DIR
#                    together with the sum
#
#  -perl             writes md5 output such that it can be pasted into this
#                    program for future reference
#
#  -newperl          similar, but only for things that we don't yet have
#
#
# Known issues:
# -------------
# A main weakness is what will happen should we change the output
# format of floating points numbers (e.g. for jet pts, etc.)]
# (or if the compiler changes this)
#
# $Id$
# ----------------------------------------------------------------------
use Digest::MD5 qw(md5 md5_hex md5_base64);
use Cwd;


# set up the location of the data & other defaults (e.g. nev)
&setDefaults;
# set up the reference results
&setRefResults;

# now allow user to play with things
while ($arg = shift @ARGV) {
  if    ($arg eq "-nev"     ) {$nev = shift @ARGV;}
  elsif ($arg eq "-alg"     ) {@algs = (shift @ARGV);}
  elsif ($arg eq "-R"       ) {$R = shift @ARGV;}
  elsif ($arg eq "-deposit" ) {$deposit = shift @ARGV;}
  elsif ($arg eq "-perl"    ) {$perlOut = "Perl Output:\n";}
  elsif ($arg eq "-newperl" ) {$perlOut = "New Perl Output:\n";}
  elsif ($arg eq "-strat")    {$defstrat = shift @ARGV;}
  else  {die "unrecognized argument $arg";}
}


# other settings

# now get the md5 sums
foreach $alg (@algs) {
if ($defstrat ne "") {
  @strat = ($defstrat);
}
elsif (exists($strategies{$alg})) {
  @strat = split(":",$strategies{$alg});
} else {
  @strat = ("")
}
foreach $strat (@strat) {

  # decide what output to use (jets for cone algs, unique_write for cam, sequence for others)
  $out = &isCone($alg) ? "-incl 0" : (($alg =~ /^cam/) ? "-unique_write" : "-write");

  # get the command line
  ($algsp = $alg) =~ s/:/ /g;
  if ($strat ne "") {$strat = "-strategy $strat"}
  $cmdline = "example/fastjet_timing_plugins -$algsp $strat -R $R $out -nev $nev 2>\&1 < $dataFile";
  $strat =~ s/.*y /s/; # we'll need this in a clean form later
  $res = `$cmdline`;

  # process the results into some decent form
  if ($res eq "" ) {
    $sum = "unavailable";
  } else {
    # remove all non-numerical lines [since these may change across versions]
    $filtered = "";
    foreach $line (split("\n",$res)) {
      if ($line =~ /^ *[0-9]/) {$filtered .= $line."\n";}
    }
    $sum = $filtered eq "" ? "unavailable" :  md5_hex($filtered)
  }

  # now generate output
  $name = &fullName($alg);
  if (exists($refResults{$name}) && $sum ne "unavailable") {
    $OK = ($sum eq $refResults{$name}) ? "OK" : "*** BAD ***";
  } else { 
    $OK = "-";
    if ($sum ne "unavailable") {$refResults{$name} = $sum;}
  }
  printf ("%-60s %-4s %-32s %s\n", $name, $strat, $sum, $OK);


  # record things for future, as perl code
  if ($sum ne "unavailable" && !exists($done{$name}) &&
      ($perlOut =~ /^Perl/s || 
       ($perlOut =~ /^New Perl/s && !exists($refResultsOrig{$name})) )) {
    $perlOut .= "  \"$name\" => \"$sum\",\n"
  }

  # optionally record things for future, in a file
  if ($deposit && !exists($done{$name}) && $sum ne "unavailable") {
    $depfile = "$deposit/$name.res";
    print "          > $depfile\n";
    open (DEP, "> $depfile") || die "Could not open $depfile";
    print DEP $res;
    close DEP;
    system("gzip -f $depfile");
    open (SUM, "> $deposit/$name.sum") || die "Could not open $deposit/$name.sum";
    print SUM  "date ".`date`;
    print SUM  "machine: ".`uname -a`;
    print SUM  "directory: ".getcwd."\n";
    print SUM  "configured: ".`egrep '^ +\\\$' config.log | head -1`;
    print SUM  "cmdline: $cmdline\n";
    print SUM  "md5sum: ",$sum,"\n";
    close SUM;
  }

  $done{$name} = 1;
}
}

if ($perlOut) {print $perlOut;}


#======================================================================
sub isCone {
  (my $alg) = @_;
  return ($alg =~ /cone/i || $alg =~ /midpoint/ || $alg =~ /jetclu/)
}


#======================================================================
sub fullName {
  (my $alg) = @_;
  ($dataTail= $dataFile) =~ s/.*\///;
  #$sep = "\@";
  $sep = ",";
  my $res = sprintf("$dataTail%snev%d%s$alg%sR%.2f",$sep,$nev,$sep,$sep, $R);
  return $res;
}

#======================================================================
sub setDefaults {

  $gavinHome = `echo ~salam`;
  chomp $gavinHome;
  $dataDir="$gavinHome/work/fastjet/data";
  $dataFile="$dataDir/Pythia-PtMin50-LHC-1000ev.dat";

  @algs = ("kt", "cam", "antikt", "genkt:0.5", "siscone:-f:0.75","siscone:-f:0.50",  "jetclu", "pxcone",
           "d0runiicone", "eekt", "eegenkt:0",  "eegenkt:-1");

  # for some algorithms we have multiple strategies to test
  %strategies = 
    (
     "kt"  => "1:-4:-3:-1:2",
     "cam" => "1:-4:-3:-1:2:12",
    );

  $defstrat = "";

  $nev = 10;
  $R = 0.6;

  $perlOut = "";
  $deposit = "";

  %done = ();

}



#======================================================================
sub setRefResults {
  %refResults = (
  # 1 ev results
  "Pythia-PtMin50-LHC-1000ev.dat,nev1,d0runiicone,R0.60" => "722f70cbb66fd5a5ee2a8a7733649daf",
  "Pythia-PtMin50-LHC-1000ev.dat,nev1,kt,R0.60" => "cca70ee3afa680bf574d94ccd6d48185",
  "Pythia-PtMin50-LHC-1000ev.dat,nev1,cam,R0.60" => "e3632acedd6516ed9c8eccfe515154dc",
  "Pythia-PtMin50-LHC-1000ev.dat,nev1,antikt,R0.60" => "13d2795fb237b1cd3459777eda8b1a19",
  "Pythia-PtMin50-LHC-1000ev.dat,nev1,genkt:0.5,R0.60" => "3da35b3292637395c41773535e145282",
  "Pythia-PtMin50-LHC-1000ev.dat,nev1,siscone:-f:0.75,R0.60" => "2d517cc2b23aad18c4afc8b58f99d842",
  "Pythia-PtMin50-LHC-1000ev.dat,nev1,siscone:-f:0.50,R0.60" => "54fd76a24330cf77658ebba9371ad8c5",
  "Pythia-PtMin50-LHC-1000ev.dat,nev1,jetclu,R0.60" => "05781404302f156dac9171e37bc09d44",
  "Pythia-PtMin50-LHC-1000ev.dat,nev1,eekt,R0.60" => "8caea0f93458e54c5418b757793c50d2",
  "Pythia-PtMin50-LHC-1000ev.dat,nev1,eegenkt:0,R0.60" => "2308c88202e0b0087c256c8a65efb5ce",
  "Pythia-PtMin50-LHC-1000ev.dat,nev1,eegenkt:-1,R0.60" => "21753b7bab26ddb06f75b8eb7b1d3024",
  # 10 ev results
  "Pythia-PtMin50-LHC-1000ev.dat,nev10,genkt:0.5,R0.60" => "f3befadd9c96dab695310b994dc5cda5",
  "Pythia-PtMin50-LHC-1000ev.dat,nev10,eekt,R0.60" => "1ab6f607fc1acec7c4aa337307347ea0",
  "Pythia-PtMin50-LHC-1000ev.dat,nev10,kt,R0.60"           =>    "d0b1a74bdcdcb9d18b8a129fb4789baa"  ,
  "Pythia-PtMin50-LHC-1000ev.dat,nev10,cam,R0.60" => "2d5b08cb9e07c0af81763865c37f8538",
  "Pythia-PtMin50-LHC-1000ev.dat,nev10,antikt,R0.60"       =>    "5ac9c93e9478cd583d2984b23e026af2"  ,
  "Pythia-PtMin50-LHC-1000ev.dat,nev10,siscone:-f:0.75,R0.60"=>    "5a1463f29a19fd368cc2821edc39f99b",
  "Pythia-PtMin50-LHC-1000ev.dat,nev10,siscone:-f:0.50,R0.60"=>    "2a88c96f6410bf721bdfc48046ac9be1",
  "Pythia-PtMin50-LHC-1000ev.dat,nev10,jetclu,R0.60"       =>    "49a43a2db9fe715b47b4e80daacf8edc",
  "Pythia-PtMin50-LHC-1000ev.dat,nev10,d0runiicone,R0.60" => "f60a1928e1b584078611a257100a99a6",
  "Pythia-PtMin50-LHC-1000ev.dat,nev10,eegenkt:0,R0.60" => "e77d363d2ea067bd62d98e7b62bba772",
  "Pythia-PtMin50-LHC-1000ev.dat,nev10,eegenkt:-1,R0.60" => "aec87012ed66823e2f30862f57307d93",
  # 100 ev results
  "Pythia-PtMin50-LHC-1000ev.dat,nev100,kt,R0.60" => "15b23d6d954c796682214d598f526f6a",
  "Pythia-PtMin50-LHC-1000ev.dat,nev100,cam,R0.60" => "9530ca5b1e1680bd2da6714fa42a13a6",
  "Pythia-PtMin50-LHC-1000ev.dat,nev100,antikt,R0.60" => "66bb86601eaf4daa50aa601af175e8a3",
  "Pythia-PtMin50-LHC-1000ev.dat,nev100,genkt:0.5,R0.60" => "6663b7c49724212f279ae3407bf6c8cd",
  "Pythia-PtMin50-LHC-1000ev.dat,nev100,siscone:-f:0.75,R0.60" => "8a6cb5d7533dcd8553fefb9dbe38e344",
  "Pythia-PtMin50-LHC-1000ev.dat,nev100,siscone:-f:0.50,R0.60" => "ba6fdee02d2fb12d6d49267c428190b9",
  "Pythia-PtMin50-LHC-1000ev.dat,nev100,jetclu,R0.60" => "8948d26cacb802bf5e2e6c1cf89dabff",
  "Pythia-PtMin50-LHC-1000ev.dat,nev100,d0runiicone,R0.60" => "a7430e1528ffcf20b3098959c7aa257d",
  "Pythia-PtMin50-LHC-1000ev.dat,nev100,eekt,R0.60" => "7388d3917738969fb7fdf518af0dca78",
  "Pythia-PtMin50-LHC-1000ev.dat,nev100,eegenkt:0,R0.60" => "c210893596b56046573c60603e4a3e5b",
  "Pythia-PtMin50-LHC-1000ev.dat,nev100,eegenkt:-1,R0.60" => "905525747cad0344b6826514cd4cc618",
  # 1000 ev results
  "Pythia-PtMin50-LHC-1000ev.dat,nev1000,kt,R0.60" => "ae0b4d26e5244cfb551d41097fad3543",
  "Pythia-PtMin50-LHC-1000ev.dat,nev1000,cam,R0.60" => "ed14ae31b5ebd2df39dafae3f800040f",
  "Pythia-PtMin50-LHC-1000ev.dat,nev1000,antikt,R0.60" => "d4a7e6146a9856e5be1cf67d59e1ee44",
  "Pythia-PtMin50-LHC-1000ev.dat,nev1000,genkt:0.5,R0.60" => "22b8a8f3126f7b11d9f42cfe3bc20d73",
  "Pythia-PtMin50-LHC-1000ev.dat,nev1000,siscone:-f:0.75,R0.60" => "e2333f97f0b69d858ec33ee8537882bf",
  "Pythia-PtMin50-LHC-1000ev.dat,nev1000,siscone:-f:0.50,R0.60" => "65f5ca86e1db411f55892831ca6aad0b",
  "Pythia-PtMin50-LHC-1000ev.dat,nev1000,jetclu,R0.60" => "57ff3c751ab1d2f82673cd28fdb9e991",
  "Pythia-PtMin50-LHC-1000ev.dat,nev1000,d0runiicone,R0.60" => "8687da1a072268000a5c0d132fb37596",
  "Pythia-PtMin50-LHC-1000ev.dat,nev1000,eekt,R0.60" => "59fe2638f8df87a48bda2b8d1490034e",
  "Pythia-PtMin50-LHC-1000ev.dat,nev1000,eegenkt:0,R0.60" => "48cb5d5a8a5f636d07569745e5be29e4",
  "Pythia-PtMin50-LHC-1000ev.dat,nev1000,eegenkt:-1,R0.60" => "e54ecd5d535f2f3d7ddffc1bfd43462c"
);

  %refResultsOrig = %refResults;
}  
