#!/bin/bash
echo
echo -----------------------------------------------------------
echo "Comparing output from these runs (test-script-output.txt) "
echo "to the expected output (test-script-output-orig.txt)"
echo -----------------------------------------------------------
grep -v 'CGAL' output.tmp > test-script-output.txt
diff --ignore-matching-lines="release" --ignore-matching-lines="SISCone" test-script-output.txt test-script-output-orig.txt > output.tmp
DIFF=`diff --ignore-matching-lines="release" --ignore-matching-lines="SISCone"  test-script-output.txt test-script-output-orig.txt`
if [[ -n $DIFF ]]; then 
  cat output.tmp
  exit 1;
else
  echo Results are identical
  rm output.tmp
  rm test-script-output.txt
fi
