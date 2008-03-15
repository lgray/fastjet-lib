#!/bin/bash

# Note on the structure of the output file, its original version we have
# to compare with and te comparison itself:
#
# orig output looks as follows:
#  FastJet banner
#  jets from k_t example
#  FastJet banner
#  Output from the area example (k_t clustering)
#    This ends with 'Number of unclustered particles: 0'
#  FastJet banner
#  jets from CDF midpoint
#    Ending with '138 particles unclustered' and a blank line
#  jets from SISCone
#    Ending with '0 particles unclustered' and a blank line
#  jets from k_t
#    Ending with '0 particles unclustered' and a blank line
#  jets from cam
#    Ending with '0 particles unclustered' and a blank line
#  jets from antikt
#    Ending with '0 particles unclustered' and a blank line
# 
# This script output will have the form
#  FastJet banner
#  jets from k_t example
#  FastJet banner
#  Output from the area example (k_t clustering)
#    This ends with 'Number of unclustered particles: 0'
#  FastJet banner
#  jets from PxCone        (IF PxCone ENABLED)
#    Ending with '??? particles unclustered' and a blank line
#  jets from CDF midpoint  (IF CDF MidPoint ENABLED)
#    Ending with '??? particles unclustered' and a blank line
#  jets from SISCone       (IF SISCone ENABLED)
#    Ending with '??? particles unclustered' and a blank line
#  jets from k_t
#    Ending with '??? particles unclustered' and a blank line
#  jets from cam
#    Ending with '??? particles unclustered' and a blank line
#  jets from antikt
#    Ending with '??? particles unclustered' and a blank line
#
# Note: in the original script, parts related to the plugins
#       are prefixed by <tag>:

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
echo -----------------------------------------------------------
echo "Running 'fastjet_example < data/single_event.dat'"
echo -----------------------------------------------------------
example/fastjet_example < ${srcdir}/example/data/single-event.dat > output.tmp

echo
echo -----------------------------------------------------------
echo "Running 'fastjet_areas < data/single_event.dat'"
echo -----------------------------------------------------------
example/fastjet_areas < ${srcdir}/example/data/single-event.dat >> output.tmp

echo
echo -----------------------------------------------------------
echo "Running 'many_algs_example < data/single_event.dat'"
echo -----------------------------------------------------------
plugins/usage_examples/many_algs_example < ${srcdir}/example/data/single-event.dat >> output.tmp

echo
echo -----------------------------------------------------------
echo "Comparing output from these runs (test-script-output.txt) "
echo "to the expected output (test-script-output-orig.txt)"
echo -----------------------------------------------------------

# we need cleaning before that:
###############################
# 1. get the installed plugins
#    and remove the unwanted lines in the original output
pluins_grep_opts=""
cp ${srcdir}/test-script-output-orig.txt output_orig.tmp
chmod u+w output_orig.tmp
if [[ -n `grep "define ENABLE_PLUGIN_PXCONE" include/fastjet/config_auto.h` ]]; then
    tested_plugins=${tested_plugins}"PxCone "
else
    grep -v -e'^pxcone:' output_orig.tmp > output_orig_tmp.tmp
    mv output_orig_tmp.tmp output_orig.tmp
fi
if [[ -n `grep "define ENABLE_PLUGIN_CDFCONES" include/fastjet/config_auto.h` ]]; then
    tested_plugins=${tested_plugins}"CDFMidPoint "
else
    grep -v -e'^cdfmp:' output_orig.tmp > output_orig_tmp.tmp
    mv output_orig_tmp.tmp output_orig.tmp
fi
if [[ -n `grep "define ENABLE_PLUGIN_SISCONE" include/fastjet/config_auto.h` ]]; then
    tested_plugins=${tested_plugins}"SISCone "
else
    grep -v -e'^siscone:' output_orig.tmp > output_orig_tmp.tmp
    mv output_orig_tmp.tmp output_orig.tmp
fi
if [[ -n `grep "define ENABLE_PLUGIN_D0RUNIICONE" include/fastjet/config_auto.h` ]]; then
    tested_plugins=${tested_plugins}"D0RunIICone "
else
    grep -v -e'^d0runiicone:' output_orig.tmp > output_orig_tmp.tmp
    mv output_orig_tmp.tmp output_orig.tmp
fi

# 2. clear the orig output
#     (i)  avoid line with '#' or 'SISCone' in them
#     (ii) remove the plugins tags
grep -v -e '#' -e 'SISCone' output_orig.tmp | sed -e "s/^pxcone://g" -e "s/^siscone://g" -e "s/^cdfmp://g" -e "s/^d0runiicone://g" > output_orig.tmp.tmp
mv output_orig.tmp.tmp output_orig.tmp

# 2. clean the 'make check' output
grep -v 'CGAL' output.tmp > test-script-output.txt
grep -v -e '#' -e 'SISCone' test-script-output.txt > output.tmp

# 3. perform the diff
DIFF=`diff output.tmp output_orig.tmp`
diff output.tmp output_orig.tmp > test-script-output.tmp
rm output.tmp output_orig.tmp

# 4. show result
echo "Tested plugins: "${tested_plugins}
if [[ -n $DIFF ]]; then 
  cat test-script-output.tmp
  exit 1;
else
  echo Results are identical
  rm test-script-output.tmp
  rm test-script-output.txt
fi
