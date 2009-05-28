#!/usr/bin/perl -w
#
# Script to help us perform a nightly check of fastjet
#
# -mail sends mail, otherwise output goes to screen
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
#   - include an "executive summary" either at the end (screen)
#     or the beginning (mail), at least when things are OK.
#     E.g.: host, options, #OK, #unavail
#
#   - include info on svn revision, and directory status?
#
#   - on svn update, check if nightly-check.pl has changed,
#     and if so, rerun
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
$mailAddr='salam@lpthe.jussieu.fr cacciari@lpthe.jussieu.fr gsoyez@quark.phy.bnl.gov'; #  g@gavin.fr 
#$configOpts="--enable-cgal --enable-d0runiicone --enable-trackjet";
#$configOpts="--enable-shared --enable-cgal --enable-d0runiicone --enable-trackjet --enable-atlascone --enable-jade";

# $configOpts="--enable-cgal --with-cgaldir=".$ENV{CGAL_DIR}." --enable-allcxxplugins";
# $fjlibOpts="";
# $nevTestAll=1000;

@setups = ();
# for each setup we put the host ("" is current host), the config
# options, the special link-time flags, and the number of events
#
# The things we want to test are:
#
# - out of the box compilation on linux
# - the same on a mac
# - a full set of algs on toth, mac, a 64 bit machine, gcc 4.4
# - shared/static issues (depending on current defaults)
# - cgal
# - at least one run with 10^3 events



push @setups, ["karnak","--enable-allcxxplugins CC=/usr/local/bin/gcc-4.4 CXX=/usr/local/bin/g++-4.4", "", 10]; # full set with gcc 4.4 
# push @setups, ["","", "", 10]; # out of the box
# push @setups, ["","--enable-allcxxplugins --enable-cgal --with-cgaldir=".$ENV{CGAL_DIR}, "", 1000]; # with CGAL & all plugins
# push @setups, ["","--enable-allcxxplugins --enable-shared --disable-static", "--runpath", 10]; # with dynlibs
# push @setups, ["","--enable-allcxxplugins --enable-shared", "--shared=no", 10]; # with static libs even though shared are built
# push @setups, ["zetes", "", "", 10]; # out of the box on zetes (SLC4, gcc 3.4.6, 64 bit)
# push @setups, ["karnak","", "", 10]; # out of the box on karnak (OS X 10.5)
# push @setups, ["karnak","--enable-allcxxplugins --enable-shared", "", 10]; # full monty on karnak
# push @setups, ["hercule","--enable-allcxxplugins", "", 10]; # hercule: standard machine, 64 bits



# /Volumes/Lacie/cacciari/local/bin/g++-4.4


# process command-line
$mail=0;
$tmpDir="";
$remote=0;
$command=$0;
$origDir=getcwd();
$tarName="";
while ($arg = shift @ARGV) {
  if ($arg eq "-mail")   {$mail = 1;}
  elsif ($arg eq "-remote") {$tmpDir  = shift @ARGV; $remote=1;}
  elsif ($arg eq "-tar")    {$tarName = shift @ARGV;}
  elsif ($arg eq "-orig")   {
    $origDir = shift @ARGV;
  }
  else {die "Unrecognized argument: $arg";}
}

# for some remote hosts, need to remove leading "/misc?" from directory name
$origDir=~ s/^\/misc//; 

$fail="";
$failDetails="";
$allMessages = "";
$verbose = ! ($mail || $remote);
#$tarName="fastjet-2.4-devel.tar.gz"; # TMP 

$uname = `uname -a`; chomp $uname;
&message("* running on $uname\n");

MAIN: while (1) {

  if (!$tmpDir) {
    #--- make tmpDir -------------------------------------------------------
    #$tmpDir = "$origDir/tmp-nightly";
    $tmpDir = "$origDir/tmp-".$$;
    &message("* making tmp directory $tmpDir\n");
    if (-e $tmpDir || ! (mkdir $tmpDir)) {
      $fail = "* creating tmp directory";
      $failDetails = "$tmpDir already exists or could not be created; stopping";
      $tmpDir = "";
      last;
    }

    #--- svn update --------------------------------------------------------
    &message("* running svn update\n");
    $svnup=`svn update 2>&1`;
    if ($svnup =~ /external .. revision [0-9]/i && 
        ($svnup =~ /^At revision [0-9]/m || $svnup =~ /^Updated to revision [0-9]/m) &&
        $svnup !~ /conflict/i) {
      # all is OK, do nothing
    } else {
      $fail = "svn update";
      $failDetails = $svnup;
      last;
    }

    #--- make dist ------------------------------------------------------
    &message("* running make dist");
    $makedist=`make dist 2>&1`;
    if ($makedist =~ / error[: ]/i || $makedist !~ />(.*?tar.gz)/) {
      $fail = "make dist";
      $failDetails = $makedist;
      &message("\n");
      last;
    } else {
      $tarName = $1;
      &message(" -> $tarName\n");
    }

    # now run the rest, either remotely, or from setups array, or from a setup file
    for ($i = 0; $i <= $#setups; $i++) {
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
        $ssh =~ s/^.*slocate.db.*\n//mg;    # because zetes has out of date locate
        $ssh =~ s/^.*updatedb.*\n//mg; # (which I use on logon...)
        if ($ssh || $?) {
          $fail = "connection to $setups[$i][0]";
          $failDetails = $ssh;
          last MAIN;
        }

        # collect the results
        $results=`cat $tmpDir/messages 2>&1`;
        if (!$results || $results =~ /Failed/ || $?) {
          $fail = "execution on remote host";
          $failDetails = $results;
          last MAIN;
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


#-- mention where failure might arise
if ($fail) {
  &message("Failed on $fail\n\nDetailed message is:\n------------------\n");
  &message($failDetails);
  &message("\n--------------- END OF FAILURE MESSAGE ---------------\n");
  $mailSubject='fastjet nightly: FAILED on '.$fail;
} elsif (!$remote) {
  &message("\nAll tests passed\n");
  # try to get more info about test results
  @unavail = split("unavailable",$allMessages);
  @areOK   = split("OK",$allMessages);
  $mailSubject='fastjet nightly: '.sprintf("%d",$#areOK).' OK';
  if ($#unavail >= 0) {$mailSubject .= ", ".sprintf("%d",$#unavail)." NA"}
}

# clean up
if ($tmpDir && !$fail && !$remote) { 
  &message("* removing $tmpDir\n");
  system("rm -rf $tmpDir")
};

# send mail if relevant, or deposit a message for the program that called us
if ($mail) {
  open (MAIL, "|mail -s '$mailSubject' $mailAddr") || die "could not open pipe for mail message";
  print MAIL $allMessages;
  close MAIL;
} elsif ($remote) {
  open (MSG, "> $tmpDir/messages") || die "Remote host could not write to $tmpDir/messages";
  print MSG $allMessages;
  close MSG;
}



#======================================================================
sub message ($) {
  (my $msg) = @_;
  $allMessages .= $msg;
  if ($verbose) {print $msg;}
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

  # $cxx = "g++";
  # # special compilers are deduced from the configure flag
  # if ($config =~ /CXX=([^\s]+)/) { $cxx = $1; }
  # &message("* compiling fastjet_timing_plugins externally (with $cxx, fastjet-config ... $link)\n");

  chdir $tmpDir;

  #--- clean up from previous invocation --
  if (-e "build/") {
    &message("\n* removing everything from the tmp dir\n");
    system("rm -rf *");
  }

  #--- untar -----------------
  &message("* untarring $origDir/$tarName in tmp dir\n");
  $untar=`tar zxvf $origDir/$tarName`;
  if ($?) {
    $fail = "untar";
    $failDetails = $untar;
    return 0;
  }

  #--- configure -----------------
  system("mkdir build/");
  chdir "build";
  &message("* running configure $config --prefix=$tmpDir/inst\n");
  ($distDir=$tarName) =~ s/.tar.gz//;
  $configOut=`../$distDir/configure $config --prefix=$tmpDir/inst 2>&1`;
  if ($configOut =~ /error[: ]/i || $?) {
    $fail = "configure";
    $failDetails = $configOut;
    return 0;
  }

  #--- run make -------------------
  &message("* running make\n");
  $make=`make -j2 2>&1`;
  if ($make =~ /error[: ]/i || $?) {
    $fail = "make";
    $failDetails = $make;
    return 0;
  }

  #--- run make check -------------------
  &message("* running make check\n");
  $makecheck=`make check 2>&1`;
  if ($makecheck =~ /error[: ]/i || $?) {
    $fail = "make check";
    $failDetails = $makecheck;
    return 0;
  }
  
  #--- run make install -------------------
  &message("* running make install\n");
  $makeinstall=`make install 2>&1`;
  if ($makeinstall =~ /error[: ]/i || $?) {
    $fail = "make install";
    $failDetails = $makeinstall;
    return 0;
  }

  #--- do external compilation -------------------
  chdir "../";
  $cxx = "g++";
  # special compilers are deduced from the configure flag
  if ($config=~ /CXX=([^\s]+)/) { $cxx = $1; }
  &message("* compiling fastjet_timing_plugins externally (with $cxx, fastjet-config ... $link)\n");
  $compile=`$cxx -O -I$distDir/example $distDir/example/CmdLine.cc $distDir/example/fastjet_timing_plugins.cc  \`inst/bin/fastjet-config --cxxflags --libs --plugins $link\` -o fastjet_timing_plugins 2>&1`;
  if ($compile =~ /error[: ]/i || $?) {
    $fail = "external compilation";
    $failDetails = $compile;
    return 0;
  }

  #--- do test-all-algs -------------------
  &message("* testing all algs\n");
  # use the original test-all-algs.pl prog, since it isn't distributed
  # in the tarball
  $testall=`../regression-tests/test-all-algs.pl -nev $nev`;
  if ($testall =~ /\sBAD/i || $testall !~ /OK/ || $?) {
    $fail = "testing all algs";
    $failDetails = $testall;
    return 0;
  } else {
    &message($testall);
  }

  return 1;
}
