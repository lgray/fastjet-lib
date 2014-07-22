#!/bin/bash
#
# assume everything has been built

function report_failure {
    echo '******* FAILED ******** '
    echo '  on execution of' $1
    exit -1
}

function header {
    echo
    echo "================================================================"
}

header
./PJtiming -n 1000 -sz 1000 || report_failure PJtiming

header
./cs_delete_self < ../example/data/single-event.dat ||  report_failure cs_delete_self

header 
./run_tests || report_failure run_tests


