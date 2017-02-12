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
sed 's/\(AC_INIT.*\)])/\1-'$extralabel'])/' < configure.ac > configure.ac.new
mv configure.ac.new configure.ac

# now make sure the windows config file is consistent
pushd src
./genconfig.sh ../include/fastjet/config_win.h
popd

# now make and test the distribution 
make -j2 distcheck 

# and put the configure file back to where it was
svn revert configure.ac
svn revert include/fastjet/config_win.h

# and tell the user you have a result
echo "**************************************************"
echo "Have produced the file(s):"
filename=`ls -rt *$extralabel*.tar.gz | tail -1`
filebase=`echo $filename | sed 's/.tar.gz//'`
echo Now run 
echo scp -p $filename tycho.lpthe.jussieu.fr:'~'salam/www/fastjet/repository/snapshots/
echo svn cp . svn+ssh://tycho.lpthe.jussieu.fr/ada1/lpthe/salam/svn/fastjet/tags/snapshots/$filebase -m \'tagged $filebase snapshot\'
echo \# edit '~'salam/www/fastjet3/snapshots.html on tycho
#mv -v *$extralabel*.tar.gz ~salam/www/fastjet/repository/snapshots/
