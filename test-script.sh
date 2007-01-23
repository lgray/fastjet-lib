#!/bin/bash
# Script for testing whether everything works!


# build the main package
pushd src
make || exit -1
make install
popd

# build the plugins
pushd plugins
make || exit -1
popd

# build the examples and test a couple
pushd example
make fastjet_example  || exit -1
./fastjet_example < data/single-event.dat || exit -1
make fastjet_areas || exit -1
./fastjet_areas < data/single-event.dat || exit -1
make fastjet_example_v1_interface || exit -1
make fastjet_subtraction || exit -1
make fastjet_timing || exit -1
#make fastjet_timing_plugins || exit -1
popd

# try out some plugin examples
pushd plugins/usage_examples
make cdfmidpoint_example many_algs_example pxcone_example siscone_example || exit -1
./many_algs_example < data/single-event.dat || exit -1
popd
