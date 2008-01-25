#!/bin/sh
# Script for generating a minimal config.h file independently of automake
# It must contain a version string, and what else?

configfile=${1:-../include/fastjet/config.h}
echo Generating $configfile
#exit

packname=`grep '^ *AC_INIT' ../configure.ac | sed -e 's/AC_INIT(//' -e 's/\[//g' -e 's/\]//g' -e 's/)//'`

# get the package string
echo '#define PACKAGE_STRING  "'$packname'"' | sed 's/,/ /g' > $configfile

# get the package string
packver=`echo $packname | sed 's/.*,//g'`
echo '#define PACKAGE_VERSION  "'$packver'"'  >> $configfile

# by default some plugins are define/disabled
cat >> $configfile <<EOF

/* The CDFJetClu and CDFMidPoint plugins are enabled by default*/
#define ENABLE_PLUGIN_CDFCONES 

/* The PxCone plugin is disabled by default*/
#undef ENABLE_PLUGIN_PXCONE

/* The SISCone plugin is enabled by default*/
#define ENABLE_PLUGIN_SISCONE 
EOF

