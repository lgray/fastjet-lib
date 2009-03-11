#!/usr/bin/perl -w
#
# script to help us perform a nightly check of fastjet
#
# -mail sends mail, otherwise, verbose output
#
#

# Items are
#   - svn update
#   - make dist
#   - untar
#   - configure from separate dir
#   - make
#   - make check
#   - regression-tests/test-all-algs.pl -nev 1000

# NB: $? is command status (non-zero with error)

use Cwd;
use English;
$OUTPUT_AUTOFLUSH = 1;

# things to configure
$mailAddr='salam@lpthe.jussieu.fr cacciari@lpthe.jussieu.fr gsoyez@quark.phy.bnl.gov'; #  g@gavin.fr 
#$configOpts="--enable-cgal --enable-d0runiicone --enable-trackjet";
#$configOpts="--enable-shared --enable-cgal --enable-d0runiicone --enable-trackjet --enable-atlascone --enable-jade";
$configOpts="--enable-shared --enable-cgal --with-cgaldir=".$ENV{CGAL_DIR}." --enable-allcxxplugins";
$nevTestAll=1000;
$mail=0;

# process command-line
while ($arg = shift @ARGV) {
  if ($arg eq "-mail") {$mail = 1;}
  else {die "Unrecognized argument: $arg";}
}

$origDir=getcwd();
$fail="";
$failDetails="";
$allMessages = "";
$verbose = !$mail;
#$tarName="fastjet-2.4-devel.tar.gz"; # TMP 

while (1) {

  #--- make tmpDir -------------------------------------------------------
  $tmpDir = "$origDir/tmp-nightly";
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

  ###--- svn update --------------------------------------------------------
  ##&message("* running svn update\n");
  ##$svnup=`svn update 2>&1`;
  ##if ($svnup =~ /external .. revision [0-9]/i && 
  ##    ($svnup =~ /^At revision [0-9]/m || $svnup =~ /^Updated to revision [0-9]/m) &&
  ##    $svnup !~ /conflict/i) {
  ##  # all is OK, do nothing
  ##} else {
  ##  $fail = "svn update";
  ##  $failDetails = $svnup;
  ##  last;
  ##}

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

  #--- untar -----------------
  &message("* untarring in tmp dir\n");
  chdir $tmpDir;
  $untar=`tar zxvf $origDir/$tarName`;
  if ($?) {
    $fail = "untar";
    $failDetails = $untar;
    last;
  }

  #--- configure -----------------
  system("mkdir build/");
  chdir "build";
  &message("* running configure $configOpts\n");
  ($distDir=$tarName) =~ s/.tar.gz//;
  $config=`../$distDir/configure $configOpts --prefix=$tmpDir/inst 2>&1`;
  if ($config =~ /error[: ]/i || $?) {
    $fail = "configure";
    $failDetails = $config;
    last;
  }


  #--- run make -------------------
  &message("* running make\n");
  $make=`make -j2 2>&1`;
  if ($make =~ /error[: ]/i || $?) {
    $fail = "make";
    $failDetails = $make;
    last;
  }

  #--- run make check -------------------
  &message("* running make check\n");
  $makecheck=`make check 2>&1`;
  if ($makecheck =~ /error[: ]/i || $?) {
    $fail = "make check";
    $failDetails = $makecheck;
    last;
  }
  
  #--- run make install -------------------
  &message("* running make install\n");
  $makeinstall=`make install 2>&1`;
  if ($makeinstall =~ /error[: ]/i || $?) {
    $fail = "make install";
    $failDetails = $makeinstall;
    last;
  }

  #--- do external compilation -------------------
  chdir "../";
  &message("* compiling fastjet_timing_plugins externally\n");
  $compile=`g++ -O -I$distDir/example $distDir/example/CmdLine.cc $distDir/example/fastjet_timing_plugins.cc \`inst/bin/fastjet-config --cxxflags\` \`inst/bin/fastjet-config --libs --plugins\` -o fastjet_timing_plugins 2>&1`;
  if ($compile =~ /error[: ]/i || $?) {
    $fail = "external compilation";
    $failDetails = $compile;
    last;
  }

  #--- do test-all-algs -------------------
  &message("* testing all algs\n");
  # use the original test-all-algs.pl prog, since it isn't distributed
  # in the tarball
  $testall=`../regression-tests/test-all-algs.pl -nev $nevTestAll`;
  if ($testall =~ /BAD/i || $?) {
    $fail = "testing all algs";
    $failDetails = $testall;
    last;
  } else {
    &message($testall);
  }


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


sub message ($) {
  (my $msg) = @_;
  $allMessages .= $msg;
  if ($verbose) {print $msg;}
}
