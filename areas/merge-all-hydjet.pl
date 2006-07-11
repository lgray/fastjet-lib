#!/usr/bin/perl -w
#
# script to merge all files in hydjet_incplt appropriately...

chdir "hydjet_inclpt";


%bases=();


open(FILELIST, "ls *_iseq*|") || die "Could not do dir listing";

while ($file = <FILELIST>) {
  chomp($file);
  ($base = $file) =~ s/\_iseq.*//;
  #print $base."\n";
  $bases{$base} = 1;
}

foreach $base (keys %bases) {
  
  print "------ Merging ".$base."_iseq*\n";
  system("../merge-hist.pl ".$base."_iseq* > $base");
}
chdir "..";
