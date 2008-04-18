#!/bin/bash

# check whether all is committed
if [[ `svn status | grep -v -e '^X' -e external | egrep -e '^[A-Z]' | wc -l` -gt 0 ]] ; then
 echo "ERROR: working copy has local changes"
 exit -1
fi

# get an up to date version
svn update || exit -1

# get the revision number
rev=`svn info | grep Revision | sed s'/[^0-9]*//'`

# extra label will be
extralabel=`date +"%Y%m%d"`-rev$rev
echo $extralabel
sed -i 's/\(AC_INIT.*\)])/\1-'$extralabel'])/' configure.ac

# now make and test the distribution 
make -j2 distcheck 

# and put the configure file back to where it was
svn revert configure.ac

# and tell the user you have a result
echo "**************************************************"
echo "Have produced the file(s):"
ls -l *$extralabel*.tar.gz
