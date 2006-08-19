#!/usr/bin/perl -w

# script to help convert programs using to old (v1) interface over to the
# new (v2) interface...

while ($line = <STDIN>) {
  if ($line =~ /using namespace std;/) {
    print "namespace fj = fastjet;\n"
  }
  $line =~ s:"Fj(.*)\.hh":"fastjet/$1.hh":;
  $line =~ s:ClusterSequenceWithArea:ClusterSequenceActiveAreaExplicitGhosts:g;
  $line =~ s:ClusterSequenceWithMeanArea:ClusterSequenceActiveArea:g;
  $line =~ s:ClusterSequenceWithPassiveArea:ClusterSequencePassiveArea:g;
  $line =~ s:"fastjet/(ClusterSequencePassiveArea):"$1:;
  $line =~ s/Fj/fj::/g;
  $line =~ s/cell/ghost/g;
  $line =~ s/Best/fj::Best/g;
  $line =~ s/([a-z]+_algorithm)/fj::$1/g;
  $line =~ s:cmdline.double_val\("-ghost_area",0.01\):cmdline.double_val\("-ghost_area",cmdline.double_val\("-cell_area",0.01\)\):;
  $line =~ s:area_err\(:area_error\(:g;
  print $line;
}

print STDERR "Remember to sort out active_area_spec and ghost,cell area\n";
