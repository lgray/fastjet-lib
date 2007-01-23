#!/bin/bash
# Script for testing whether everything works!


# set G77LIBDIR if the user has not set it in its environment
if [[ ! $G77LIBDIR ]] ; then
    dir=`locate libg2c.a | head -1`
    G77LIBDIR=`dirname $dir`/
    echo Setting G77LIBDIR to $G77LIBDIR
fi


# set CLEAN to "empty string" in order NOT to clean
CLEAN=""

#if [ $# > 0 ];then
#if [ $1 == "-c" ]; then
#CLEAN="1"
#fi
#fi


# build the main package
pushd src
if [ $CLEAN ]; then make clean || exit -1; fi
make install
popd

# build the plugins
pushd plugins
if [ $CLEAN ]; then make clean || exit -1; fi
make || exit -1
popd

# build the examples and test a couple
pushd example
if [ $CLEAN ]; then make clean || exit -1; fi
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
if [ $CLEAN ]; then make clean || exit -1; fi
make cdfmidpoint_example many_algs_example pxcone_example siscone_example || exit -1
./many_algs_example < data/single-event.dat || exit -1
popd
