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
#                    [things separated by a : become separated by a space
#                    in the final command]
#
#  -R   R            sets R
#
#  -strat STRAT      use only this strategy [1 == best]; multiple colon-separated 
#                    strategies may also be specified
#
#  -deposit DIR      puts the (unfiltered) output in a directory DIR
#                    together with the sum
#
#  -perl             writes md5 output such that it can be pasted into this
#                    program for future reference
#
#  -newperl          similar, but only for things that we don't yet have
#
# Full (non-md5) results of a 1000 event run are to be found in
# the (non svn) directory
#
#     ~salam/work/fastjet/validation-ref-2008-10-30
#
#
# Known issues:
# -------------
# A main weakness is what will happen should we change the output
# format of floating points numbers (e.g. for jet pts, etc.)]
# (or if the compiler changes this)
#
#
# Adding algorithms:
# ------------------
#
# - make sure that "-newalg" (or whatever it's called) runs the new
#   algorithm in fastjet_timing_plugins. 
#
# - run "./test-all-algs.pl -alg newalg -nev {1|10|100|1000} -newperl"
#
#   Extra options can be given too: "newalg:-y:0.8" will run
#   fastjet_timing_plugins with options "-newalg -y 0.8".
#
#   the {1|10|100|1000} means you should carry out separate runs with
#   1, 10, 100, 1000 events, so as to get the checksums for each.
#
#   Each time you'll get a line of perl that is to be added to the
#   initialisation of the %refResults hash (in setRefResults()).
#
# - now rerun the command above, and check that the algorithm is
#   labelled "OK" on each run
#
# - run "./test-all-algs.pl -alg newalg -nev 1000 -deposit SOME-DIRECTORY" 
#
#   that will place the raw results in SOME-DIRECTORY (I use
#   ~salam/work/fastjet/validation-ref-2008-10-30 -- ideally  everything
#   end up in the same place)
#
# - add "newalg" to the @algs array in setDefaults()
#
# - if you want things to be tested in the nightly build, make sure
#   the new algorithm is actually compiled -- i.e. add the appropriate
#   configure options to $configOpts in nightly-check.pl
#
# - commit and then run nightly-check.pl (nightly-check.pl
#   deliberately fails on uncommitted directories -- to avoid giving
#   results based on something not actually in the repository)
#
# - The next automatic run of nightly-check.pl will use an old
#   nightly-check.pl script (it runs the script from a special
#   directory and the script does the update only after starting...)
#   and so your new algorithm won't necessarily be configured (it will
#   then be labelled as unavailable -- or NA in the subject line).
#
#   It's only the following night that things will reach
#   "equilibrium".
#
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
  elsif ($arg eq "-strat" || $arg eq "-strategy")    {$defstrat = shift @ARGV;}
  else  {die "unrecognized argument $arg";}
}


# other settings

# now get the md5 sums
foreach $alg (@algs) {
if ($defstrat ne "") {
  @strat = split(":",$defstrat);
}
elsif (exists($strategies{$alg})) {
  @strat = split(":",$strategies{$alg});
} else {
  @strat = ("")
}
foreach $strat (@strat) {

  # decide what output to use (jets for cone algs, unique_write for cam, sequence for others)
  $out = &isCone($alg) ? "-incl 0" : (($alg =~ /^cam/) ? "-unique_write" : "-write");

  # decide from which file we get the events 
  $localdataFile = &isee($alg) ? $eedataFile : $dataFile;

  # get the command line
  ($algsp = $alg) =~ s/:/ /g;
  if ($strat ne "") {$strat = "-strategy $strat"}
  #$cmdline = "$execName -$algsp $strat -R $R $out -nev $nev 2>\&1 < $localdataFile";
  $cmdline = "$execName -$algsp $strat -R $R $out -nev $nev 2>\&1";
  if ($localdataFile =~ /\.gz$/) {
    $cmdline = "zcat $localdataFile | $cmdline";
  } else {
    $cmdline = "$cmdline < $localdataFile ";
  }
  $strat =~ s/.*y /s/; # we'll need this in a clean form later
  $res = `$cmdline`;
  $error = $?;

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
  if ($error) {
    $OK = "*** BAD (crash?) ***"
  } elsif (exists($refResults{$name}) && $sum ne "unavailable") {
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
    $configlog = "config.log";
    if (! -e $configlog) {$configlog = "../".$configlog;}
    print SUM  "configured: ".`egrep '^ +\\\$' $configlog | head -1`;
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
sub isee {
  (my $alg) = @_;
  return ($alg =~ /^ee/i || $alg =~ /jade/ )
}


#======================================================================
sub fullName {
  (my $alg) = @_;
  
  # decide from which file we get the events 
  $localdataFile = &isee($alg) ? $eedataFile : $dataFile;
  ($dataTail= $localdataFile) =~ s/.*\///;
  $dataTail =~ s/\.gz//;

  #$sep = "\@";
  $sep = ",";
  my $res = sprintf("$dataTail%snev%d%s$alg%sR%.2f",$sep,$nev,$sep,$sep, $R);
  return $res;
}

#======================================================================
sub setDefaults {

  $username=`whoami`;
  chomp $username;
  if ( $username eq "greg"){
      $dataDir="~/diska/jets/fastjet/data";
  } else {
      $gavinHome = `echo ~salam`;
      chomp $gavinHome;
      $dataDir="$gavinHome/work/fastjet/data";
  }
  #$dataFile="$dataDir/Pythia-PtMin50-LHC-1000ev.dat";
  $dataFile="$dataDir/Pythia-PtMin50-LHC-10kev.dat.gz";

  # for the e+e- algorithms, use an e+e- event file
  $eedataFile="$dataDir/Pythia_Q1000_Zprime1000_nev1000.dat";

  @algs = ("kt", "cam", "antikt", "genkt:0.5", "siscone:-f:0.75","siscone:-f:0.50",  "jetclu", "pxcone",
            "d0runiicone", #GPS removed 2010-01-19, replace 2010-02-02
	   "eekt", "eegenkt:0",  "eegenkt:-1", "eecambridge:-ycut:0.08", "eecambridge:-ycut:0.01",
	   "trackjet", "atlascone", "cmsiterativecone", "jade:-excly:0.01", "d0runicone", "d0runipre96cone");


  # for some algorithms we have multiple strategies to test
  %strategies = 
    (
     "kt"  => "1:-4:-3:-1:2",
     "antikt"  => "1:-4:-3:-1:2",
     "cam" => "1:-4:-3:-1:2:12",
    );


  # find out which executable to use based on what's locally
  # available, and failing that based on where we are
  if (-x "fastjet_timing_plugins") {
    $execName = getcwd."/fastjet_timing_plugins"
  } elsif (-x "example/fastjet_timing_plugins") {
    $execName = getcwd."/example/fastjet_timing_plugins"
  } else {
    $execName  =  getcwd."/$0";
    $execName  =~ s/regression-tests.*//;
    $execName .=  "example/fastjet_timing_plugins";
  }
  print "Using $execName\n\n";

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
  "Pythia-PtMin50-LHC-10kev.dat,nev1,d0runiicone,R0.60" => "722f70cbb66fd5a5ee2a8a7733649daf",
  "Pythia-PtMin50-LHC-10kev.dat,nev1,kt,R0.60" => "cca70ee3afa680bf574d94ccd6d48185",
  "Pythia-PtMin50-LHC-10kev.dat,nev1,cam,R0.60" => "e3632acedd6516ed9c8eccfe515154dc",
  "Pythia-PtMin50-LHC-10kev.dat,nev1,antikt,R0.60" => "13d2795fb237b1cd3459777eda8b1a19",
  "Pythia-PtMin50-LHC-10kev.dat,nev1,genkt:0.5,R0.60" => "3da35b3292637395c41773535e145282",
  "Pythia-PtMin50-LHC-10kev.dat,nev1,siscone:-f:0.75,R0.60" => "2d517cc2b23aad18c4afc8b58f99d842",
  "Pythia-PtMin50-LHC-10kev.dat,nev1,siscone:-f:0.50,R0.60" => "54fd76a24330cf77658ebba9371ad8c5",
  "Pythia-PtMin50-LHC-10kev.dat,nev1,jetclu,R0.60" => "05781404302f156dac9171e37bc09d44",
  "Pythia-PtMin50-LHC-10kev.dat,nev1,pxcone,R0.60" => "c39185086e8ad3e35d13d32f2d03c41a",
  # ee algs ran on pp events
  #"Pythia-PtMin50-LHC-10kev.dat,nev1,eekt,R0.60" => "8caea0f93458e54c5418b757793c50d2",
  #"Pythia-PtMin50-LHC-10kev.dat,nev1,eegenkt:0,R0.60" => "d1d52a0e1b45b11590cab257c5af5152",
  #"Pythia-PtMin50-LHC-10kev.dat,nev1,eegenkt:-1,R0.60" => "8923f3b1d1e3b3e58859e7363ad5f538",
  "Pythia_Q1000_Zprime1000_nev1000.dat,nev1,eekt,R0.60" => "6a7c5a8ec3700a82343fe597235c8fd7",
  "Pythia_Q1000_Zprime1000_nev1000.dat,nev1,eegenkt:0,R0.60" => "d251c9efd3c4c304d646aabb252d4280",
  "Pythia_Q1000_Zprime1000_nev1000.dat,nev1,eegenkt:-1,R0.60" => "df18bdb90f1088c7b3b572915a5c9b85",
  "Pythia_Q1000_Zprime1000_nev1000.dat,nev1,eecambridge:-ycut:0.08,R0.60" => "ccbb772f1af5102aa59fc923f3e625cc",
  "Pythia_Q1000_Zprime1000_nev1000.dat,nev1,eecambridge:-ycut:0.01,R0.60" => "cd039768d90103dbdc6dd2722abe712e",
  # old R def for eegenkt
  #"Pythia-PtMin50-LHC-10kev.dat,nev1,eegenkt:0,R0.60" => "2308c88202e0b0087c256c8a65efb5ce",
  #"Pythia-PtMin50-LHC-10kev.dat,nev1,eegenkt:-1,R0.60" => "21753b7bab26ddb06f75b8eb7b1d3024",
  "Pythia-PtMin50-LHC-10kev.dat,nev1,trackjet,R0.60" => "144b09c5042300633e2f687e16ffc17a",
  "Pythia-PtMin50-LHC-10kev.dat,nev1,atlascone,R0.60" => "929de8bd58c1bc7fc5cbbef781031ad3",
  "Pythia-PtMin50-LHC-10kev.dat,nev1,cmsiterativecone,R0.60" => "d789dbd3daf07e747ca4541e109c8e0a",
  "Pythia_Q1000_Zprime1000_nev1000.dat,nev1,jade:-excly:0.01,R0.60" => "77ef328afb881e59866d8e72298b019b",
  "Pythia-PtMin50-LHC-10kev.dat,nev1,d0runicone,R0.60" => "49a06117d12d018c1c8aebbce38918fd",
  "Pythia-PtMin50-LHC-10kev.dat,nev1,d0runipre96cone,R0.60" => "ceb8fecf1bced37b378b749cf05a673c",

  # 10 ev results
  "Pythia-PtMin50-LHC-10kev.dat,nev10,genkt:0.5,R0.60" => "f3befadd9c96dab695310b994dc5cda5",
  "Pythia-PtMin50-LHC-10kev.dat,nev10,eekt,R0.60" => "1ab6f607fc1acec7c4aa337307347ea0",
  "Pythia-PtMin50-LHC-10kev.dat,nev10,kt,R0.60"           =>    "d0b1a74bdcdcb9d18b8a129fb4789baa"  ,
  "Pythia-PtMin50-LHC-10kev.dat,nev10,cam,R0.60" => "2d5b08cb9e07c0af81763865c37f8538",
  "Pythia-PtMin50-LHC-10kev.dat,nev10,antikt,R0.60"       =>    "5ac9c93e9478cd583d2984b23e026af2"  ,
  "Pythia-PtMin50-LHC-10kev.dat,nev10,siscone:-f:0.75,R0.60"=>    "5a1463f29a19fd368cc2821edc39f99b",
  "Pythia-PtMin50-LHC-10kev.dat,nev10,siscone:-f:0.50,R0.60"=>    "2a88c96f6410bf721bdfc48046ac9be1",
  "Pythia-PtMin50-LHC-10kev.dat,nev10,jetclu,R0.60"       =>    "49a43a2db9fe715b47b4e80daacf8edc",
  "Pythia-PtMin50-LHC-10kev.dat,nev10,pxcone,R0.60" => "f25494f08feb0b4398d45e6b2bf60b0a",
  "Pythia-PtMin50-LHC-10kev.dat,nev10,d0runiicone,R0.60" => "f60a1928e1b584078611a257100a99a6",
  # ee algs ran on pp events
  #"Pythia-PtMin50-LHC-10kev.dat,nev10,eegenkt:0,R0.60" => "2c95f0f347a3ba4b35255ffc20da13d5",
  #"Pythia-PtMin50-LHC-10kev.dat,nev10,eegenkt:-1,R0.60" => "07aec8542d7687e0465958d3f2e5e86e",
  "Pythia_Q1000_Zprime1000_nev1000.dat,nev10,eekt,R0.60" => "7e8b864013f28ee0f4d8cbebe79abbd3",
  "Pythia_Q1000_Zprime1000_nev1000.dat,nev10,eegenkt:0,R0.60" => "79ea7d89f47427c3b348e6ff9bba3a66",
  "Pythia_Q1000_Zprime1000_nev1000.dat,nev10,eegenkt:-1,R0.60" => "faac5c2857ed616cd420060e7675466c",
  "Pythia_Q1000_Zprime1000_nev1000.dat,nev10,eecambridge:-ycut:0.08,R0.60" => "9e248c74c5c8b3729f1e96a538daf146",
  "Pythia_Q1000_Zprime1000_nev1000.dat,nev10,eecambridge:-ycut:0.01,R0.60" => "12f7dab534711378bf6809e5e31f13d3",
  # old R def for eegenkt
  #"Pythia-PtMin50-LHC-10kev.dat,nev10,eegenkt:0,R0.60" => "e77d363d2ea067bd62d98e7b62bba772",
  #"Pythia-PtMin50-LHC-10kev.dat,nev10,eegenkt:-1,R0.60" => "aec87012ed66823e2f30862f57307d93",
  "Pythia-PtMin50-LHC-10kev.dat,nev10,trackjet,R0.60" => "c6bbf392b1a1712256f260d7aeb52267",
  "Pythia-PtMin50-LHC-10kev.dat,nev10,atlascone,R0.60" => "212c22dd6f1901657a2ff7df0c07badc",
  "Pythia-PtMin50-LHC-10kev.dat,nev10,cmsiterativecone,R0.60" => "069f4c6693f154e67e0e49cd9fc29786",
  "Pythia_Q1000_Zprime1000_nev1000.dat,nev10,jade:-excly:0.01,R0.60" => "876ee902b4680c3ec1ef189a82308927",
  "Pythia-PtMin50-LHC-10kev.dat,nev10,d0runicone,R0.60" => "1c6eb67467c3354de464748d8bf67680",
  "Pythia-PtMin50-LHC-10kev.dat,nev10,d0runipre96cone,R0.60" => "178140344e6f6d536ee4f2b1e1ff1a13",

  # 100 ev results
  "Pythia-PtMin50-LHC-10kev.dat,nev100,kt,R0.60" => "15b23d6d954c796682214d598f526f6a",
  "Pythia-PtMin50-LHC-10kev.dat,nev100,cam,R0.60" => "9530ca5b1e1680bd2da6714fa42a13a6",
  "Pythia-PtMin50-LHC-10kev.dat,nev100,antikt,R0.60" => "66bb86601eaf4daa50aa601af175e8a3",
  "Pythia-PtMin50-LHC-10kev.dat,nev100,genkt:0.5,R0.60" => "6663b7c49724212f279ae3407bf6c8cd",
  "Pythia-PtMin50-LHC-10kev.dat,nev100,siscone:-f:0.75,R0.60" => "8a6cb5d7533dcd8553fefb9dbe38e344",
  "Pythia-PtMin50-LHC-10kev.dat,nev100,siscone:-f:0.50,R0.60" => "ba6fdee02d2fb12d6d49267c428190b9",
  "Pythia-PtMin50-LHC-10kev.dat,nev100,jetclu,R0.60" => "8948d26cacb802bf5e2e6c1cf89dabff",
  "Pythia-PtMin50-LHC-10kev.dat,nev100,pxcone,R0.60" => "a3781182e4a363725b0929023dc27628",
  # result before addition of -fno-inline on 32 bit machines
  #"Pythia-PtMin50-LHC-10kev.dat,nev100,d0runiicone,R0.60" => "a7430e1528ffcf20b3098959c7aa257d",
  # result with inclusion of that (should be consistent across 32 and 64 bits)
  "Pythia-PtMin50-LHC-10kev.dat,nev100,d0runiicone,R0.60" => "7058932e29b3979f4159a384a880770b",
  # ee algs ran on pp events
  #"Pythia-PtMin50-LHC-10kev.dat,nev100,eekt,R0.60" => "7388d3917738969fb7fdf518af0dca78",
  #"Pythia-PtMin50-LHC-10kev.dat,nev100,eegenkt:0,R0.60" => "f4c2804090331b997a1f4fe1ea10ee61",
  #"Pythia-PtMin50-LHC-10kev.dat,nev100,eegenkt:-1,R0.60" => "59e20c82081eb5d584156b82175e9162",
  "Pythia_Q1000_Zprime1000_nev1000.dat,nev100,eekt,R0.60" => "ca2228232a53941082b1e0c4d4b91e28",
  "Pythia_Q1000_Zprime1000_nev1000.dat,nev100,eegenkt:0,R0.60" => "3bf1d61b8837bb42bfc1c00ad9ec1606",
  "Pythia_Q1000_Zprime1000_nev1000.dat,nev100,eegenkt:-1,R0.60" => "dd3222176571e51a4bb4d8d9876dea51",
  "Pythia_Q1000_Zprime1000_nev1000.dat,nev100,eecambridge:-ycut:0.08,R0.60" => "b248aa2ad20544720df26c3886e11e5c",
  "Pythia_Q1000_Zprime1000_nev1000.dat,nev100,eecambridge:-ycut:0.01,R0.60" => "420d8b18e3ecbdd6de1fed5f3533e811",
  # old R def for eegenkt
  #"Pythia-PtMin50-LHC-10kev.dat,nev100,eegenkt:0,R0.60" => "c210893596b56046573c60603e4a3e5b",
  #"Pythia-PtMin50-LHC-10kev.dat,nev100,eegenkt:-1,R0.60" => "905525747cad0344b6826514cd4cc618",
  "Pythia-PtMin50-LHC-10kev.dat,nev100,trackjet,R0.60" => "bc388ed856a9f20ce6336201ce8f6afa",
  "Pythia-PtMin50-LHC-10kev.dat,nev100,atlascone,R0.60" => "d5bc5a6427cc7af883b2329e45ee0d7a",
  "Pythia-PtMin50-LHC-10kev.dat,nev100,cmsiterativecone,R0.60" => "afd780e095e04bce14dc8587d0736e48",
  "Pythia_Q1000_Zprime1000_nev1000.dat,nev100,jade:-excly:0.01,R0.60" => "7ce00dc7a5f676552c447fce5b8a0197",
  "Pythia-PtMin50-LHC-10kev.dat,nev100,d0runicone,R0.60" => "62b7fc59816298800cb5aac3b3424d1c",
  "Pythia-PtMin50-LHC-10kev.dat,nev100,d0runipre96cone,R0.60" => "ba82d640e1763ad03956ee9428cb67e3",

  # 1000 ev results
  "Pythia-PtMin50-LHC-10kev.dat,nev1000,kt,R0.60" => "ae0b4d26e5244cfb551d41097fad3543",
  "Pythia-PtMin50-LHC-10kev.dat,nev1000,cam,R0.60" => "ed14ae31b5ebd2df39dafae3f800040f",
  "Pythia-PtMin50-LHC-10kev.dat,nev1000,antikt,R0.60" => "d4a7e6146a9856e5be1cf67d59e1ee44",
  "Pythia-PtMin50-LHC-10kev.dat,nev1000,genkt:0.5,R0.60" => "22b8a8f3126f7b11d9f42cfe3bc20d73",
  "Pythia-PtMin50-LHC-10kev.dat,nev1000,siscone:-f:0.75,R0.60" => "e2333f97f0b69d858ec33ee8537882bf",
  "Pythia-PtMin50-LHC-10kev.dat,nev1000,siscone:-f:0.50,R0.60" => "65f5ca86e1db411f55892831ca6aad0b",
  "Pythia-PtMin50-LHC-10kev.dat,nev1000,jetclu,R0.60" => "57ff3c751ab1d2f82673cd28fdb9e991",
  # pxcone differs between 32/64 bit machines (and compilers?); the answer here is
  # for a 64 bit gfortran
  "Pythia-PtMin50-LHC-10kev.dat,nev1000,pxcone,R0.60" => "a15a20876c52578b9b4b07a96b67a13a",
  # result before addition of -fno-inline on 32 bit machines
  #"Pythia-PtMin50-LHC-10kev.dat,nev1000,d0runiicone,R0.60" => "8687da1a072268000a5c0d132fb37596",
  # result with inclusion of that (should be consistent across 32 and 64 bits)
  "Pythia-PtMin50-LHC-10kev.dat,nev1000,d0runiicone,R0.60" => "26796bd9e42aa4b2ccae678074714613",
  # ee algs ran on pp events
  #"Pythia-PtMin50-LHC-10kev.dat,nev1000,eekt,R0.60" => "59fe2638f8df87a48bda2b8d1490034e",
  #"Pythia-PtMin50-LHC-10kev.dat,nev1000,eegenkt:0,R0.60" => "f282981b9ff8e2db65fa0dd7aa1e9e44",
  #"Pythia-PtMin50-LHC-10kev.dat,nev1000,eegenkt:-1,R0.60" => "719609342c170c7ef74e758f1cc28988",
  "Pythia_Q1000_Zprime1000_nev1000.dat,nev1000,eekt,R0.60" => "5d88b51cdc8581ddffe3110081f3c3dd",
  "Pythia_Q1000_Zprime1000_nev1000.dat,nev1000,eegenkt:0,R0.60" => "acd7c1540ddf0889ea73d7daddc1a590",
  "Pythia_Q1000_Zprime1000_nev1000.dat,nev1000,eegenkt:-1,R0.60" => "3a5cd000d227f72752e4d5bc98a28e3d",
  "Pythia_Q1000_Zprime1000_nev1000.dat,nev1000,eecambridge:-ycut:0.08,R0.60" => "00eb9436e2f8458c01b495c58e5b30d3",
  "Pythia_Q1000_Zprime1000_nev1000.dat,nev1000,eecambridge:-ycut:0.01,R0.60" => "cb9ea9ba8b46cbbe4f663cd34e53c8f1",
  # old R def for eegenkt
  #"Pythia-PtMin50-LHC-10kev.dat,nev1000,eegenkt:0,R0.60" => "48cb5d5a8a5f636d07569745e5be29e4",
  #"Pythia-PtMin50-LHC-10kev.dat,nev1000,eegenkt:-1,R0.60" => "e54ecd5d535f2f3d7ddffc1bfd43462c",
  "Pythia-PtMin50-LHC-10kev.dat,nev1000,trackjet,R0.60" => "865e8763a52f63e43bb5ac781a8087f1",
  "Pythia-PtMin50-LHC-10kev.dat,nev1000,atlascone,R0.60" => "5efdfffa446604f043c0444bc09e0f8a",
  "Pythia-PtMin50-LHC-10kev.dat,nev1000,cmsiterativecone,R0.60" => "31a543ee68e64eb67d5b242188cb7aab",
  "Pythia_Q1000_Zprime1000_nev1000.dat,nev1000,jade:-excly:0.01,R0.60" => "b4aef5930856daafb294ddce66834bc7",
  "Pythia-PtMin50-LHC-10kev.dat,nev1000,d0runicone,R0.60" => "a475ca9a5bdcf9278ccfe8854095dfef",
  "Pythia-PtMin50-LHC-10kev.dat,nev1000,d0runipre96cone,R0.60" => "d9e503ea1cb13767ca5decf53bb001d1"
);

  %refResultsOrig = %refResults;
}  
