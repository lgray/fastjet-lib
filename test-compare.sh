#!/bin/bash

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
grep -v 'CGAL' output.tmp > test-script-output.txt

grep -v -e '#' -e 'SISCone' test-script-output.txt > output1.tmp
grep -v -e '#' -e 'SISCone' test-script-output-orig.txt > output2.tmp
DIFF=`diff output1.tmp output2.tmp`
diff output1.tmp output2.tmp > output.tmp
rm output1.tmp output2.tmp

#diff --ignore-matching-lines="release" --ignore-matching-lines="Written" --ignore-matching-lines="SISCone" test-script-output.txt test-script-output-orig.txt > output.tmp
#DIFF=`diff --ignore-matching-lines="release" --ignore-matching-lines="Written" --ignore-matching-lines="SISCone"  test-script-output.txt test-script-output-orig.txt`

if [[ -n $DIFF ]]; then 
  cat output.tmp
  exit 1;
else
  echo Results are identical
  rm output.tmp
  rm test-script-output.txt
fi
