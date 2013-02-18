#!/bin/bash

fjdir=..
version=$(grep "AC_INIT" $fjdir/configure.ac | sed 's/^AC_INIT(\[.*\],\[//;s/\])$//')

mkdir fjcore-$version
cd fjcore-$version
fjdir=../$fjdir

internal_headers="base.hh\
  numconsts.hh\
  IsBase.hh\
  SearchTree.hh\
  DynamicNearestNeighbours.hh\
  MinHeap.hh\
  ClosestPair2DBase.hh\
  ClosestPair2D.hh"

fastjet_headers="config_auto.h\
  config.h\
  version.hh\
  SharedPtr.hh\
  Error.hh\
  LimitedWarning.hh\
  PseudoJetStructureBase.hh\
  PseudoJet.hh\
  FunctionOfPseudoJet.hh\
  Selector.hh\
  JetDefinition.hh\
  CompositeJetStructure.hh\
  ClusterSequenceStructure.hh\
  ClusterSequence.hh"

internal_sources="ClusterSequence_N2.icc"

fastjet_sources="ClosestPair2D.cc\
  ClusterSequence.cc\
  ClusterSequence_CP2DChan.cc\
  ClusterSequence_DumbN3.cc\
  ClusterSequence_N2.cc\
  ClusterSequenceStructure.cc\
  ClusterSequence_TiledN2.cc\
  CompositeJetStructure.cc\
  Error.cc\
  FunctionOfPseudoJet.cc\
  JetDefinition.cc\
  LimitedWarning.cc\
  MinHeap.cc\
  PseudoJet.cc\
  PseudoJetStructureBase.cc\
  Selector.cc"


# create the directory structure
mkdir include
mkdir include/fastjet
mkdir include/fastjet/internal
mkdir src

# copy the internal headers, headers and sources
echo "======================================================================"
echo "copying internal headers"
for hh in $internal_headers; do
    cp $fjdir/include/fastjet/internal/$hh include/fastjet/internal/
done
echo "copying FastJet headers"
for hh in $fastjet_headers; do
    cp $fjdir/include/fastjet/$hh include/fastjet/
done
echo "copying internal sources"
for icc in $internal_sources; do
    cp $fjdir/include/fastjet/internal/$icc include/fastjet/internal/
done
echo "copying FastJet sources"
for cc in $fastjet_sources; do
    cp $fjdir/src/$cc src/
done

# create a rough Makefile (for testing)
echo "======================================================================"
echo "creating a temporary Makefile"
TAB="$(printf '\t')"

cat >Makefile <<EOF
all:
${TAB}@cd src && \$(MAKE)
clean:
${TAB}@cd src && \$(MAKE) clean
EOF

cat >src/Makefile <<EOF
SRCS = $fastjet_sources
OBJS =  \$(patsubst %.cc,%.o,\$(SRCS))
CFLAGS = -Wall -Woverloaded-virtual -ansi -pedantic -Wextra -Wshadow -O3 -g -DDROP_CGAL -D__FJCORE__ -Wl,--enable-new-dtags -I../include

%.o: %.cc
${TAB}g++ -c \$(CFLAGS) \$<

all: \$(OBJS) 
${TAB}ar cru libfjcore.a \$(OBJS)
${TAB}ranlib libfjcore.a

clean:
${TAB}rm -f *~ *.o
EOF

# try to build the library
echo; echo "building the extracted code"
make

# then, for each header, try to include it and build dit against the lib
echo; echo "checking individual headers (version expected to fail)"
for hh in $fastjet_headers; do
    echo $hh
    cat >tmp.cc <<EOF
#include "include/fastjet/${hh}"

int main(){
  return 0;
}
EOF
    g++ -Lsrc -lfjcore -I. -Iinclude -Wall -ansi -pedantic -Wshadow -Wextra -DDROP_CGAL -D__FJCORE__ tmp.cc
done

# now merge everything in a single headre and a single source
echo "======================================================================"
echo "Merging all the headers into fjcore.hh"
cat >fjcore.hh <<EOF
#ifndef __FJCORE_HH__
#define __FJCORE_HH__

#define __FJCORE__

EOF

for hh in $internal_headers; do
    cat include/fastjet/internal/$hh >> fjcore.hh
done

for hh in $fastjet_headers; do
    cat include/fastjet/$hh >> fjcore.hh
done
echo "#endif" >> fjcore.hh

# copy the source
echo; echo "Merging all the sources into fjcore.cc"
cat >fjcore.cc <<EOF
#include "fjcore.hh"
EOF

for icc in $internal_sources; do
    cat include/fastjet/internal/$icc >> fjcore.cc
done

for cc in $fastjet_sources; do
    cat src/$cc >> fjcore.cc
done

echo; echo "Cleaning the #include directives"
for pattern in $internal_headers $fastjet_headers; do
    grep -v "include.*$pattern" fjcore.hh > tmp
    mv tmp fjcore.hh
    grep -v "include.*$pattern" fjcore.cc > tmp
    mv tmp fjcore.cc
done

# cd ..

