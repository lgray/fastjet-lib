#!/bin/bash

# This file aims at comparing the output of the different algorithms 
# with the benchmark results stored in test-algorithms-orig.dat
#
# To do that, we'll follow the following recipe:
#   - get the list of available algorithms
#     Note that at the same time, we'll build the info to clear the 
#     original output later on
#   - run the various algorithms using fastjet_timing_plugins
#     send the output to a tmp file
#   - clear orig and tmp files
#   - compare them
# 
# Specific notes:
#   - Tested algorithms: 
#       kt, cam, antikt, ee_kt, ee_cam, ee_antikt,
#       siscone, siscone_spheri, jetclu, midpoint, pxcone,
#       d0runiicone, trackjet
#   - What needs to be cleared in the orig file:
#       the lines starting with "<alg>:" for the algorithms
#       that won't be used
#   - What needs to be cleared in the test file:
#       . all comment lined starting with '#'
#   - For native algs, we'll fix the strategy so that the output 
#     matches (Best could be dangerous in case CGAL gets used)
#   - additional things tested: the output of fastjet_example and
#     fastjet_areas that will be placed at the beginning of the file
# 
# TODO:
#   - add genkt, ee_kt, ee_genkt
#   - vary parameters

#======================================================================
# first check that the windows include file has the correct
# version number. Strictly speaking not something that 
# should be done in make check, but should help guarantee
# that we do not release a copy with the wrong windows version 
# number
packname=`grep '^ *AC_INIT' configure.ac | sed -e 's/AC_INIT(//' -e 's/\[//g' -e 's/\]//g' -e 's/)//'`
packver=`echo $packname | sed 's/.*,//g'`
winver=`grep VERSION include/fastjet/config_win.h | sed 's/.*VERSION *"//' | sed 's/"//'`
if [[ $winver != $packver ]]; then
  echo "ERROR: config_win.h version number not compatible with true version number"
  exit 1
fi


#======================================================================
# now run the real tests
if test -z ${srcdir}; then
    echo "setting srcdir to ."
    srcdir="."
fi

# first build the list of algs to run
echo -----------------------------------------------------------
echo "Checking which algorithms are available for testing"
echo -----------------------------------------------------------
tested_algs="kt cam antikt "
untested_algs=""

echo "^#" > clear_patterns.orig
echo "CGAL" >> clear_patterns.orig
echo "SISCone" >> clear_patterns.orig  # avoids problems w version numbers
cp clear_patterns.orig clear_patterns.tmp
echo ":#" >> clear_patterns.orig

# note: algs specified as alg:name mean that 'name' has to be checked for the 
#       availability of 'alg'
for plugin_tag in siscone sisconespheri:siscone jetclu:cdfcones midpoint:cdfcones pxcone d0runiicone trackjet ; do
    plugin=${plugin_tag%%:*}
    tag=${plugin_tag##*:}

    tag_upper=`echo ${tag} | tr a-z A-Z`

    if [[ -n `grep "define ENABLE_PLUGIN_${tag_upper}" include/fastjet/config_auto.h` ]]; then
	tested_algs=${tested_algs}" "${plugin}
    else
	untested_algs=${untested_algs}" "${plugin}
	echo "^${plugin}:" >> clear_patterns.orig
    fi
done

# build the output to be compared wityh the original one
## for regenerating the orig output: echo "blahblahthiswillneverhappen" > clear_patterns.tmp
echo -----------------------------------------------------------
echo "Running 'fastjet_example < data/single_event.dat'"
echo -----------------------------------------------------------
example/fastjet_example < ${srcdir}/example/data/single-event.dat | grep -v -E -f clear_patterns.tmp > output.tmp

echo
echo -----------------------------------------------------------
echo "Running 'fastjet_areas < data/single_event.dat'"
echo -----------------------------------------------------------
example/fastjet_areas < ${srcdir}/example/data/single-event.dat | grep -v -E -f clear_patterns.tmp >> output.tmp

# run the algorithms to be tested
echo -----------------------------------------------------------
echo "Running 'fastjet_timing_plugins -incl 5.0 < data/single_event.dat' on all algs"
echo "  tested  : "${tested_algs}
echo "  untested:" ${untested_algs}
echo -----------------------------------------------------------
for alg in ${tested_algs}; do
    example/fastjet_timing_plugins -${alg} -incl 5.0 -strategy -3 < ${srcdir}/example/data/single-event.dat \
      | grep -v -E -f clear_patterns.tmp \
      | awk "{if (\$2 == \"exclusive\"){ exit;}; print \"${alg}:\"\$0}"  >> output.tmp
done
## for regenerating the orig output: cp output.tmp test-script-output-orig.txt

echo
echo -----------------------------------------------------------
echo "Comparing output from these runs (test-script-output.txt) "
echo "to the expected output (test-script-output-orig.txt)"
echo -----------------------------------------------------------
# clear the original output for comment lines and untested algorithms
grep -v -E -f clear_patterns.orig  test-script-output-orig.txt >  output_orig.tmp

# 4. perform the diff
DIFF=`diff output.tmp output_orig.tmp`
diff output.tmp output_orig.tmp > test-script-output.tmp
# rm output.tmp output_orig.tmp

# 5. show result
echo "Tested plugins: "${tested_plugins}
if [[ -n $DIFF ]]; then 
  cat test-script-output.tmp
  exit 1;
else
  echo Results are identical

  rm test-script-output.tmp
  rm clear_patterns.orig
  rm clear_patterns.tmp
fi
