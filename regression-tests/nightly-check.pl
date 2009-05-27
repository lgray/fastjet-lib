#!/usr/bin/perl -w
#
# script to help us perform a nightly check of fastjet
#
# -mail sends mail, otherwise, verbose output
#
#
#----------------------------------------------------------------------
# Items are
#   - svn update
#   - make dist
#   - untar
#   - configure from separate dir
#   - make
#   - make check
#   - regression-tests/test-all-algs.pl -nev 1000
#
#----------------------------------------------------------------------
# Future options:
#   - include various other configures (with/without shared libs, cgal, etc.)
#   - include different compilers
#     [e.g. /ada1/lpthe/cacciari/local/bin/g++-4.4]
#   - and try it out on macs too?
#
# What should the architecture be? Call this program with options?
# This program calls others if you ask for specific cases?
#
#----------------------------------------------------------------------
# Reminder notes:
#
# NB: $? is command status (non-zero with error)

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
# for each setup we put in the config options, the special link-time flags, and the number of events
push @setups, ["", "", 10]; # out of the box
push @setups, ["--enable-allcxxplugins --enable-cgal --with-cgaldir=".$ENV{CGAL_DIR}, "", 1000]; # with CGAL & all plugins
push @setups, ["--enable-allcxxplugins --enable-shared --disable-static", "--runpath", 10]; # with dynlibs
push @setups, ["--enable-allcxxplugins --enable-shared", "--static", 10]; # with static libs even though shared are built


# process command-line
$mail=0;
while ($arg = shift @ARGV) {
  if ($arg eq "-mail") {$mail = 1;}
  else {die "Unrecognized argument: $arg";}
}

$origDir=getcwd();
$fail="";
$failDetails="";
$allMessages = "";
$tmpDir = "";
$verbose = !$mail;
#$tarName="fastjet-2.4-devel.tar.gz"; # TMP 

MAIN: while (1) {

  #--- make tmpDir -------------------------------------------------------
  #$tmpDir = "$origDir/tmp-nightly";
  $tmpDir = "$origDir/tmp-".$$;
  $uname = `uname -a`; chomp $uname;
  &message("* running on $uname\n");
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

  # now run the rest
  for ($i = 0; $i <= $#setups; $i++) {
    &build_and_check($setups[$i][0], $setups[$i][1], $setups[$i][2]) || last MAIN;
  }
  #&build_and_check($configOpts, $fjlibOpts, $nevTestAll) || last;

  # now just exit
  last;
}


#-- mention where failure might arise
if ($fail) {
  &message("Failed on $fail\n\nDetailed message is:\n------------------");
  &message($failDetails);
  $mailSubject='fastjet nightly: FAILED on '.$fail;
} else {
  &message("\nAll tests passed\n");
  # try to get more info about test results
  @unavail = split("unavailable",$testall);
  @areOK   = split("OK",$testall);
  $mailSubject='fastjet nightly: '.sprintf("%d",$#areOK).' OK';
  if ($#unavail >= 0) {$mailSubject .= ", ".sprintf("%d",$#unavail)." NA"}
}

# send mail if relevant
if ($mail) {
  open (MAIL, "|mail -s '$mailSubject' $mailAddr") || die "could not open pipe for mail message";
  print MAIL $allMessages;
  close MAIL;
}

# clean up
if ($tmpDir) { 
  &message("* removing $tmpDir\n");
  system("rm -rf $tmpDir")
};


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

  #--- clean up from previous invocation --
  if (-e "build/") {
    &message("\n* removing everything from the tmp dir\n");
    system("rm -rf *");
  }

  #--- untar -----------------
  &message("* untarring in tmp dir\n");
  chdir $tmpDir;
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
  $config=`../$distDir/configure $config --prefix=$tmpDir/inst 2>&1`;
  if ($config =~ /error[: ]/i || $?) {
    $fail = "configure";
    $failDetails = $config;
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
  &message("* compiling fastjet_timing_plugins externally (with fastjet-config ... $link)\n");
  $compile=`g++ -O -I$distDir/example $distDir/example/CmdLine.cc $distDir/example/fastjet_timing_plugins.cc  \`inst/bin/fastjet-config --cxxflags --libs --plugins $link\` -o fastjet_timing_plugins 2>&1`;
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
  if ($testall =~ /\sBAD/i || $?) {
    $fail = "testing all algs";
    $failDetails = $testall;
    return 0;
  } else {
    &message($testall);
  }

  return 1;
}
