#!/usr/bin/perl -w
#
# Script to help us perform a nightly check of fastjet
#
#    -mail        sends mail to all authors, otherwise output goes to screen
#    -mailgavin   sends mail to just gavin
#    -verbose     output goes to screen even if we also ask for mail
#    -only index  runs only the setup corresponding to the index that's requested
#
# Various other options provide access to internals for running checks
# on remote hosts. The set of configurations that are run is given in
# the setups variable below.
#
#----------------------------------------------------------------------
# The script does the following once:
#   - svn update
#   - make dist
#
# And then the following for each setup
#   - untar
#   - configure from separate dir
#   - make
#   - make check
#   - regression-tests/test-all-algs.pl -nev [some number]
#
#----------------------------------------------------------------------
# Future options:
#
#   - do a build in situ
#
#   - try fastjet-config with --cxxflags and --libs in separate invocations
#
#   - add tests of things like areas, subjets (goes in fastjet_timing_plugins.cc)?
#
# For implementing "options", what one might do is add an optional
# hash entry to the setups, e.g. {BuildInSitu => 1}; then for
# transferring this between machines, use the "Storable" perl
# module.
#
#----------------------------------------------------------------------
# Reminder notes:
#
# NB: $? is command status (non-zero with error)
#
# -------- model crontab file (on toth)--------------------------------
# # select the default shell (to get all paths, etc., e.g. for CGAL)
# SHELL=/bin/zsh
#
# # at 5.34 every morning run the fastjet tests;
# #
# 34 05 * * * cd $HOME/work/jets/fjr-branches/fastjet-trunk-nightly-tests ; regression-tests/nightly-check.pl -mail

use Cwd;
use English;
$OUTPUT_AUTOFLUSH = 1;

# things to configure
$mailAddr='salam@lpthe.jussieu.fr cacciari@lpthe.jussieu.fr soyez@cern.ch'; #  g@gavin.fr 

# the CGAL path
#
# GS: it was previously using Gavin's environment variable. Until he
# updates it, we're temporarily going to use a fixed path pointing to
# my installation. Niote that CGAL is curently only tested on 64-bit
# systems, so I'll use the 64-bit build
$CGAL_DIR='/ada3/anciens/soyez/jets/utils/CGAL-3.6.1_install';

@setups = ();
# for each setup we put the host ("" is current host), the config
# options, the special link-time arguments to fastjet-config, and the
# number of events
#
# The things we want to test are:
#
# - out of the box compilation on linux
# - the same on a mac
# - a full set of algs on toth, mac, a 64 bit machine, gcc 4.4
# - shared/static issues (depending on current defaults)
# - cgal
# - at least one run with 10^3 events
# - at least one run with pxcone


push @setups, ["","", "", 10]; # out of the box
#push @setups, ["","--enable-allcxxplugins --enable-cgal --with-cgaldir=".$ENV{CGAL_DIR}, "", 1000]; # with CGAL & all plugins
push @setups, ["","--enable-allcxxplugins --enable-cgal --with-cgaldir=".$CGAL_DIR, "", 1000]; # with CGAL & all plugins
push @setups, ["","--enable-allplugins --disable-shared", "", 10]; # with static libs, and pxcone
push @setups, ["","--enable-allplugins --disable-monolithic", "", 10]; # test the non-monolithic build (all plugins in separate libs)
push @setups, ["","--enable-allcxxplugins --enable-shared", "--shared=no", 10]; # with static libs even though shared are built
push @setups, ["","--enable-allcxxplugins CC=icc CXX=icpc --disable-debug", "", 1000]; # with the intel compiler
# maintain a check with gxx-3.4
push @setups, ["","--enable-allcxxplugins CC=gcc34 CXX=g++34", "", 10]; # with gxx-3.4
push @setups, ["tycho","--enable-allcxxplugins", "", 1000]; # tycho: standard machine, 32 bits
# GPS 2010-10-13: zetes should be back up
push @setups, ["zetes", "--enable-allcxxplugins", "", 10]; # out of the box + all plugins on zetes (SLC4, gcc 3.4.6, 64 bit)
#push @setups, ["karnak","FC=/usr/local/bin/gfortran-4.4", "", 10]; # out of the box on karnak (OS X 10.5)
# karnak reenabled 2010-12-20
push @setups, ["karnak","", "", 10]; # out of the box on karnak (OS X 10.5)
push @setups, ["karnak","--enable-allcxxplugins", "", 1000]; # full monty on karnak
push @setups, ["karnak","--enable-allcxxplugins --disable-shared", "", 10]; # full monty on karnak
push @setups, ["karnak","--enable-allcxxplugins", "--shared=no", 10]; # full monty on karnak
push @setups, ["karnak","--enable-allcxxplugins CC=/usr/local/bin/gcc-4.4 CXX=/usr/local/bin/g++-4.4", "", 10]; # full set with gcc 4.4 


# process command-line
$mail=0;
$tmpDir="";
$remote=0;
$command=$0;
$commandArgs=join(" ",@ARGV);
$origDir=getcwd();
$tarName="";
$verbose="";
$only="";
while ($arg = shift @ARGV) {
  if    ($arg eq "-mail")      {$mail = 1;}
  elsif ($arg eq "-mailgavin") {$mail = 1; $mailAddr='salam@lpthe.jussieu.fr';}
  elsif ($arg eq "-verbose")   {$verbose = 1;}
  elsif ($arg eq "-only")      {$only = shift @ARGV;}
  # the following args are only for internal treatment of execution
  # on remote hosts
  elsif ($arg eq "-remote")    {$tmpDir  = shift @ARGV; $remote=1;}
  elsif ($arg eq "-tar")       {$tarName = shift @ARGV;}
  elsif ($arg eq "-orig")      {$origDir = shift @ARGV;}
  else {die "Unrecognized argument: $arg";}
}

# for some remote hosts, need to remove leading "/misc?" from directory name
$origDir=~ s/^\/misc//; 

$fail="";
$failDetails = "";
$allMessages = "";
$summary = "";
$testall = "";
$verbose = $verbose || (! ($mail || $remote));
$svnrev = "";
$date = "";
#$tarName="fastjet-2.4-devel.tar.gz"; # TMP 


MAIN: while (1) {

  if (!$remote) {
    #--- make tmpDir -------------------------------------------------------
    #$tmpDir = "$origDir/tmp-nightly";
    $tmpDir = "$origDir/tmp-".$$;
    &message("* making tmp directory $tmpDir\n");
    if (-e $tmpDir || ! (mkdir $tmpDir)) {
      $tmpDir = "";
      &fail("* creating tmp directory","$tmpDir already exists or could not be created; stopping");
    }

    #--- svn update --------------------------------------------------------
    &message("* running svn update\n");
    $svnup=`svn update 2>&1`;
    if ($svnup =~ /external .. revision [0-9]/i && 
        ($svnup =~ /^At revision [0-9]/m || $svnup =~ /^Updated to revision [0-9]/m) &&
        $svnup !~ /conflict/i) {
      # check if we had a merge -- in that case using this script is dangerous, so tell 
      # user
      if ($svnup =~ /^G..*nightly-check.pl/m) {&fail("svn update merged nightly-check.pl", $svnup);}
      # if the script was just updated, then rerun ourselves
      if ($svnup =~ /^U..*nightly-check.pl/m) {
        &message("* nightly-check.pl has been updated, rerunning\n\n");
        system("$command $commandArgs");
        last;
      }
      # all is OK, do nothing
    } else {
      &fail("svn update", $svnup);
    }
    if (($svninfo = `svn info`) =~ /^Revision: ([0-9]+)/m) {
      $svnrev = $1;
      &message("* svn revision: $svnrev\n");
    } else {
      &fail("getting svn revision",$svninfo);
    }
    $svnstatus=`svn status`;
    $svnstatus =~ s/^(\?|X|Performing status).*\n//mg;
    $svnstatus =~ s/^\n//mg;
    if ($?) {
      &fail("svn status",$svnstatus)
    } else {
      &message("* svn status:\n".$svnstatus);
    }
    # some useful stuff for the summary
    $date=`date`; chomp($date);
    $summary .= "SUMMARY: $date, svn revision $svnrev\n$svnstatus---------------------------------------------------\n\n";

    #--- make dist ------------------------------------------------------
    &message("* running make dist");
    $makedist=`make dist 2>&1`;
    if ($makedist =~ / error[: ]/i || $makedist !~ />(.*?tar.gz)/) {
      &message("\n");
      &fail ("make dist", $makedist);
    } else {
      $tarName = $1;
      &message(" -> $tarName\n");
    }

    # now run the rest, either remotely, or from setups array, or from a setup file
    for ($i = 0; $i <= $#setups; $i++) {
      if ($only ne "" && $only != $i) {next;}
      if ($setups[$i][0]) {
        # run test on a remote host 
        &message("* transferring execution to remote host $setups[$i][0]\n");

        # first set up a file on remote host with the info of interest
        open(SETUP, "> $tmpDir/setup") || die "Could not write to $tmpDir/setup";
        for ($j=1; $j <=3; $j++) {print SETUP $setups[$i][$j],"\n";}
        close SETUP;

        # connect to remote host and run there
        $ssh=`ssh $setups[$i][0] $origDir/$command -remote $tmpDir -orig $origDir -tar $tarName 2>&1`;
        $ssh =~ s/^.*in the future\n//mg;   # because karnak's time is wrong
        $ssh =~ s/^.*slocate.db.*\n//mg;    # because zetes has out-of-date locate
        $ssh =~ s/^.*updatedb.*\n//mg; # (which I use on logon...)
        if ($ssh || $?) {&fail("connection to $setups[$i][0]", $ssh);}

        # collect the results
        $results  = `cat $tmpDir/messages 2>&1`;
        $summary .= `cat $tmpDir/summary 2>&1`;
        if (!$results || $results =~ /Failed/ || $?) {
          &fail("execution on remote host", $results);
        } else {
          &message($results);
        }
      } else {

        # run the test locally
        &build_and_check($setups[$i][1], $setups[$i][2], $setups[$i][3]) || last MAIN;

      }
    }
      
  } else {
    # remote case, in which tmpDir is already there
    # read the instructions
    open(SETUP, "< $tmpDir/setup") || die "failed to read from $tmpDir/setup;";
    for ($j=0; $j <= 2; $j++) {$setup[$j] = <SETUP>; chomp($setup[$j]);}
    close SETUP;
    # execute them
    &build_and_check($setup[0], $setup[1], $setup[2]) || last MAIN;
  }

  #&build_and_check($configOpts, $fjlibOpts, $nevTestAll) || last;

  # now just exit
  last;
}

&finish();



#======================================================================
sub finish () {
  #if (!$remote) {$summary = "SUMMARY\n-------\n".$summary;}
  #-- mention where failure might arise
  if ($fail) {
    &message("Failed on $fail\n\nDetailed message is:\n------------------\n");
    &message($failDetails);
    &message("\n--------------- END OF FAILURE MESSAGE ---------------\n");
    $mailSubject='fastjet nightly: FAILED on '.$fail;
  } elsif (!$remote) {
    &message("\nAll tests passed\n");
    # try to get more info about test results
    $mailSubject = 'fastjet nightly: '.OKUnavail($allMessages);
  }

  # clean up
  if ($tmpDir && !$fail && !$remote) { 
    &message("* removing $tmpDir\n");
    system("rm -rf $tmpDir")
  };

  # send mail if relevant, or deposit a message for the program that called us
  if ($mail) {
    open (MAIL, "|mail -s '$mailSubject' $mailAddr") || die "could not open pipe for mail message";
    print MAIL $summary."\n\n";
    print MAIL $allMessages;
    close MAIL;
  } elsif ($remote) {
    open (MSG, "> $tmpDir/messages") || die "Remote host could not write to $tmpDir/messages";
    print MSG $allMessages;
    close MSG;
    open (SUM, "> $tmpDir/summary") || die "Remote host could not write to $tmpDir/summary";
    print SUM $summary;
    close SUM;
  }

  if ($verbose && !$remote) {
    print "\n\n".$summary;
  }
  
  exit;
}


#======================================================================
sub fail($$) {
  ($fail, $failDetails) = @_;
  $summary .= "   FAILED on $fail\n";
  &finish();
}

#======================================================================
sub message ($) {
  (my $msg) = @_;
  $allMessages .= $msg;
  if ($verbose) {print $msg;}
}


#======================================================================
# given an output string provide, an message containing # of OK / 
# unavailable options.
sub OKUnavail ($) {
  my ($input) = @_;
  my $output;
  @unavail = split("unavailable",$input);
  @areOK   = split("OK",$input);
  $output=sprintf("%d",$#areOK).' OK';
  if ($#unavail >= 0) {$output .= ", ".sprintf("%d",$#unavail)." NA"}
  return $output;
}

#======================================================================
#
# Untars, configures, compiles, does a link with an example program, and runs
# it to check that the output is correct
#
# - $config:       the configure-time flags
# - $link:         flags passed to fastjet-config at link time
# - $nev:          number of events to actually test
#
sub build_and_check($$$) {
  my ($config,$link,$nev) = @_;

  # get info about the compiler
  $cxx = "g++";
  # special compilers are deduced from the configure flag
  if ($config=~ /CXX=([^\s]+)/) { $cxx = $1; }
  $compiler = `$cxx --version 2>&1 | head -1`; chomp $compiler;

  # start constructing the summary
  ($host = `uname -n`) =~ s/\..*//; chomp($host);
  $shortuname = `uname -sm`; chomp($shortuname);
  $summary .= "Running on $host: $shortuname, $compiler
   config: $config
   tests:  link($link), nev($nev)
";


  chdir $tmpDir;

  #--- clean up from previous invocation --
  if (-e "build/") {
    &message("\n* removing everything from the tmp dir\n");
    system("rm -rf *");
  }

  # some detailed info about the system
  $uname = `uname -a`; chomp $uname;
  &message("* running on $uname\n");
  &message("* c++ compiler: $cxx, $compiler\n");

  

  #--- untar -----------------
  &message("* untarring $origDir/$tarName in tmp dir\n");
  $untar=`tar zxvf $origDir/$tarName`;
  if ($?) {
    &fail("untar",$untar);
  }

  #--- configure -----------------
  system("mkdir build/");
  chdir "build";
  &message("* running configure $config --prefix=$tmpDir/inst\n");
  ($distDir=$tarName) =~ s/.tar.gz//;
  $configOut=`../$distDir/configure $config --prefix=$tmpDir/inst 2>&1`;
  if ($configOut =~ /error[: ]/i || $?) {
    &fail("configure",$configOut);
  }

  # figure out the f77 compiler too
  $fcompiler="";
  if (`cat Makefile` =~ /^F(77|C) = ([^\s]+)$/m) {
    $fcompiler = $2;
    $fcompiler .= ", ".`$fcompiler --version 2>&1 | head -1`;
    chomp $fcompiler;
  }
  &message("* fortran compiler: $fcompiler\n");

  #--- run make -------------------
  &message("* running make\n");
  $make=`make -j2 2>&1`;
  # be careful about how we check for errors in case we trigger
  # intel warnings
  if ($make =~ /^[Ee]rror[: ]/ || $make =~ / [Ee]rror[: ]/ || $?) {
    &fail("make",$make);
  }

  #--- run make check -------------------
  &message("* running make check\n");
  $makecheck=`make check 2>&1`;
  if ($makecheck =~ /error[: ]/i || $?) {
    &fail("make check",$makecheck);
  }
  
  #--- run make install -------------------
  &message("* running make install\n");
  $makeinstall=`make install 2>&1`;
  if ($makeinstall =~ /error[: ]/i || $?) {
    &fail("make install",$makeinstall);
  }

  #--- do external compilation -------------------
  chdir "../";
  &message("* compiling fastjet_timing_plugins externally (with $cxx, fastjet-config ... $link)\n");
  $compile=`$cxx -O -I$distDir/example $distDir/example/CmdLine.cc $distDir/example/fastjet_timing_plugins.cc  \`inst/bin/fastjet-config --cxxflags --libs --plugins $link\` -o fastjet_timing_plugins 2>&1`;
  if ($compile =~ /error[: ]/i || $?) {
    &fail("external compilation",$compile);
  }

  #--- do test-all-algs -------------------
  &message("* testing all algs\n");
  # use the original test-all-algs.pl prog, since it isn't distributed
  # in the tarball
  $testall=`../regression-tests/test-all-algs.pl -nev $nev`;
  $summary .= "   status: ".&OKUnavail($testall)."\n\n";
  if ($testall =~ /\sBAD/i || $testall !~ /OK/ || $?) {
    &fail("testing all algs",$testall);
  } else {
    &message($testall);
  }

  return 1;
}
