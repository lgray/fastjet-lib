#!/bin/bash

fjdir=..
version=$(grep "AC_INIT" $fjdir/configure.ac | sed 's/^AC_INIT(\[.*\],\[//;s/\])$//')

mkdir fjcore-$version || { echo "A previous fjcore exists. Exiting."; exit 1; }
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

internal_sources="internal/ClusterSequence_N2.icc\
  version.hh"

fastjet_sources="ClosestPair2D.cc\
  ClusterSequence.cc\
  ClusterSequence_CP2DChan.cc\
  ClusterSequence_Delaunay.cc\
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
    cp $fjdir/include/fastjet/$icc include/fastjet/$icc
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
make || { echo "Failed"; exit 1; }
    

# then, for each header, try to include it and build dit against the lib
echo; echo "checking individual headers"
for hh in $fastjet_headers; do
    echo $hh
    if [[ "$hh" == "version.hh" ]]; then
	echo "Skipped"
	continue;
    fi
    cat >tmp.cc <<EOF
#include "include/fastjet/${hh}"

int main(){
  return 0;
}
EOF
    g++ -Lsrc -lfjcore -I. -Iinclude -Wall -ansi -pedantic -Wshadow -Wextra -DDROP_CGAL -D__FJCORE__ tmp.cc  || { echo "Failed"; exit 1; }
done
rm tmp.cc a.out

# now merge everything in a single header and a single source
echo "======================================================================"
echo "Merging all the headers into fjcore.hh"
cat >fjcore.hh <<EOF
#ifndef __FJCORE_HH__
#define __FJCORE_HH__

#define __FJCORE__   // remove all the non-core code (a safekeeper)
#define DROP_CGAL    // disable CGAL support

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
    cat include/fastjet/$icc >> fjcore.cc
done

for cc in $fastjet_sources; do
    cat src/$cc >> fjcore.cc
done

echo; echo "Cleaning the #include directives"
for pattern in $internal_headers $fastjet_headers $internal_sources; do
    grep -v "include.*$pattern" fjcore.hh > tmp
    mv tmp fjcore.hh
    grep -v "include.*$pattern" fjcore.cc > tmp
    mv tmp fjcore.cc
done

echo; echo "Cleaning the resulting file:"
wc -l fjcore.{hh,cc}
echo "  - removing ifdef'ed code"
for fn in fjcore.hh fjcore.cc; do
    awk 'BEGIN{level=0;outcore=0;elsecore=0}{if (NF==0){next;} if ($1~/^#if/){level=level+1} if ($1=="#ifndef" && $2=="__FJCORE__"){outcore=level} if (outcore==0){print $0} if ($1~/^#endif/){ if (level==outcore){elsecore=0;outcore=0} level=level-1}  if (elsecore==1){ print $0} if ($1~/^#else/ && level==outcore){elsecore=1}}' $fn > tmp
    mv tmp $fn
done
wc -l fjcore.{hh,cc}
echo "  - removing comment lines"
sed -i '/^ *\/\/.*$/d' fjcore.hh
sed -i '/^ *\/\/.*$/d' fjcore.cc
wc -l fjcore.{hh,cc}


# add preamble to fjcore.hh|cc (if done earlier, it gets canceled by comments removal)
echo; echo "Including preamble with appropriate version number"
for i in cc hh; do
  cat ../preamble.txt fjcore.$i > tmp$$
  sed -i "s/--FJVERSION--/$version/" tmp$$
  mv tmp$$ fjcore.$i
done  

# now testing the final product by compiling the examples
echo "======================================================================"
echo "Now compiling and running examples for checking:"
echo "  CC [fjcore.cc]"
g++ -c -Wall -Woverloaded-virtual -ansi -pedantic -Wextra -Wshadow -O3 -g fjcore.cc
for idx in 01 02 04 05 08 09 10; do
    fname=$(ls $fjdir/example/$idx-*.cc)
    fname=${fname##*/}

    # get the example
    cat $fjdir/example/$fname | sed 's/\/\/ENDHEADER/#include "fjcore.hh"/;s/^#include "fastjet\/.*$//g' > $fname

    echo "  CC   [$fname]"
    g++ -c -Wall -Woverloaded-virtual -ansi -pedantic -Wextra -Wshadow -O3 -g $fname || { echo "Failed."; exit 1; }
    echo "  LD   [${fname%.cc}]"
    g++ -o ${fname%.cc} -g ${fname%cc.o} fjcore.o -lm || { echo "Failed."; exit 1; }
    echo "  CHK  [${fname%.cc}] --- currently unimplemented"
    rm ${fname%.cc}*
done
    

# remove the temporary files
echo "======================================================================"
echo "Cleaning unnecessary files"
rm -Rf src include Makefile *.o

echo "======================================================================"
echo "fjcore-${version}/fjcore.{hh,cc} is now ready"
echo "======================================================================"

# cd ..

