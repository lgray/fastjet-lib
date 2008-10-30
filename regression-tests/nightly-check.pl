#!/usr/bin/perl -w
#
# script to help us perform a nightly check of fastjet

# Items are
#   - svn update
#   - make dist
#   - untar
#   - configure from separate dir
#   - make
#   - make check
#   - regression-tests/test-all-algs.pl -nev 1000

use Cwd;

$origDir=getcwd();
$fail="";
$failDetails="";
$verbose = 1;
$allMessages = "";

while (1) {

  #--- make tmpDir -------------------------------------------------------
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
  &message("* running make dist\n");
  $makedist=`make dist 2>&1`;
  if ($makedist =~ / error[: ]/i) {
    $fail = "make dist";
    $failDetails = $makedist;
    last;
  }


  # now just exit
  last;
}


if ($fail) {
  print "Failed on $fail\n\nDetailed message is:\n------------------";
  print $failDetails;
  
} else {
  print "\nAll tests passed\n";
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
