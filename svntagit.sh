#!/bin/bash

# make sure we really have the latest versions of everything
svn update

# deduce version automatically from the appropriate include file
version=`grep 'fastjet_version = ' include/fastjet/version.hh | sed 's/.* = \"//' | sed 's/\".*//'`

# figure out what the situation is with siscone
sisconeparent=plugins/SISCone/
sisconechild=$sisconeparent/siscone
sisconeURL=`svn info $sisconechild | grep URL| sed 's/^.*URL: //'`
sisconerev=`svn info $sisconechild | egrep '^Revision:' | sed 's/Revision: //'`
echo -- -r$sisconerev $sisconeURL

# reminders about what to do for svn
URL=`svn info | grep URL | sed 's/^.*URL: //'`
tagURL=`echo $URL | sed "s/trunk\/fastjet-release/tags\/fastjet-$version/"`

echo "svn mkdir $tagURL -m 'made directory for tag of release $version' "
echo "svn copy  -m 'tagged release of release $version' $URL $tagURL/"
