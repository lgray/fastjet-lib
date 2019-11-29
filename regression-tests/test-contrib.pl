#!/usr/bin/env perl
#
# Script to download fjcontrib and build it against a given fastjet version
#
# Usage:
#   test-contrib.pl {--svn URL | --path path}  [--fastjet-config path-to-fastjet-config]
use warnings;
use IO::Handle;
use Cwd;


$fjconfig=`which fastjet-config`;
$contribURL="";
$contribPath="";
$svnBase="svn+ssh://vcs\@phab.hepforge.org/source/fastjetsvn/contrib/";
#$svnBase="svn+ssh://svn.hepforge.org/hepforge/svn/fastjet/contrib/";
$tmpDir="";

while ($arg = shift @ARGV) {
  if      ($arg eq "--svn")  {$contribURL  = shift @ARGV;}
  elsif ($arg eq "--path") {$contribPath = shift @ARGV;}
  elsif ($arg eq "--fastjet-config") {$fjconfig = shift @ARGV;}
  else { die "unrecognized argument $arg";}
}

# decide where to get our contrib from
if ($contribPath && !$contribURL) {
  chdir $contribPath;
} elsif ($contribURL && !$contribPath) {
  if ($contribURL !~ /^svn\+ssh/) {$contribURL = $svnBase."$contribURL";}
  $tmpDir = "fjcontrib-tmp-".$$;
  mkdir $tmpDir;
  &runCommand("svn checkout","svn co $contribURL $tmpDir");
  chdir $tmpDir;
  &runCommand("getting all contribs","./scripts/update-contribs.sh --force");
}

# print out basic info
print "Working from ",getcwd,"\n";
system("svn info | grep -e '^URL' -e '^Revision'");

# now run the checks
&runCommand("configure","./configure --fastjet-config=$fjconfig","",\&printall);
&runCommand("make clean","make clean");
&runCommand("make","make -j4");
&runCommand("make check","make -j4 check","Failed",\&printSuccessFailure);

# clean up
if ($tmpDir) {
  chdir "../";
  system ("rm -rf $tmpDir");
}

# runCommand(testName, command, regexp_to_test_for_failure, ref_to_print_function)
sub runCommand {
  my ($testName, $command, $regexpFailure, $functionRef) = @_;
  print "\n----------------------------------------\n";
  print "Running $testName...\n";
  print "Command: $command\n";
  STDOUT->flush();
  $out = `$command`;
  if ($?) {
    print  "FAILED (bad exit code):\n";
    print  "$out";
    print  "Exiting on failure of $testName";
    exit(-1);
  } elsif ($regexpFailure && $out =~ /$regexpFailure/m) {
    print  "FAILED (output matches /$regexpFailure/m):\n";
    print  "$out";
    die "Exiting on failure of $testName";
    exit(-2);
  } else {
    if ($functionRef) {$functionRef->($out);}
    print "OK\n";
  }
}

sub printall {
  my ($text) = @_;
  print $text;
}

sub printSuccessFailure {
  my ($text) = @_;
  foreach $line (split("\n",$text)) {
    if ($line =~ /Success/ || $line =~ /Failure/) {print $line,"\n";}
  }
}
