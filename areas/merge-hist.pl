#!/usr/bin/perl -w
# script to merge histogram files together, weighted based on the 
# "nev = " line
#
# Usage: "merge-hist.pl file1 file2 [...] > output"
#

$nfile = $#ARGV + 1;
@nev     = (); $nev=0;
@HANDLES = ();

print "# $0 ".join(" ",@ARGV)."\n";

for($ifile = 0; $ifile < $nfile; $ifile++) {
  open($HANDLES[$ifile], "<$ARGV[$ifile]") || 
    die "Could not read $ARGV[$ifile]";
  $HANDLE = $HANDLES[$ifile];
  # figure out the number of events
  while ($line = <$HANDLE>) {
    if ($line =~ /nev = *([0-9]+)/) {
      $nev[$ifile]  = $1; 
      $nev         += $1;
      last;}
  }
}

print "# nev = $nev\n";
print STDERR "Total number of events is $nev\n";

while () {
  $good = 0;
  @out = ();
  for($ifile = 0; $ifile < $nfile; $ifile++) {
    $HANDLE = $HANDLES[$ifile];
    while ($good = ($line = <$HANDLE>)) {if ($line !~ /^# /) {last;}}
    if (!$good) {last;}
    chomp ($line);
    @pieces = split(/ +/, $line);
    #print join(" ",@pieces)."\n";
    for ($i = 0; $i <= $#pieces; $i++) {
      $piece = $nev[$ifile] * $pieces[$i];
      if ($ifile == 0) {$out[$i] = $piece} else {$out[$i] += $piece}
    }
  }
  if (!$good) {last;}
  for ($i = 0; $i <= $#out; $i++) {$out[$i] /= $nev};
  print join(' ', @out)."\n";
}
