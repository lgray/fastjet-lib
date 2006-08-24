#!/bin/zsh
# create a tar archive

#version=0.9.0c-20050929-1200
#version=0.9pre-20060203-2140
version=2.0.0b1
origdir=`pwd | sed 's/.*\///'`
echo "Will make an archive of $origdir/"
dir=fastjet-$version
tarname=$dir.tgz

# make sure we have Makefile with use CGAL=no
echo "Moving original Makefile out of way to make a copy with USE_CGAL = no"
mv -v Makefile Makefile.orig
cat Makefile.orig | sed 's/^USE_CGAL *= *yes/USE_CGAL = no/' > Makefile

pushd ..

if [[ -e $tarname ]]
then
  echo "Tarfile $tarname already exists. Not proceeding."
else
  echo "Creating $tarname:"
  if [[ -e $dir ]] 
  then
    echo "Could not create $dir as link to $origdir (former exists already)"
  else
    ln -s $origdir $dir
    tar zcvhf $tarname $dir/(src|include|example|doc|.)/*.(f90|f|h|hh|alg|c|cc|C|tex|eps) \
                      $dir/(src|include|example|doc)/Makefile \
                      $dir/Makefile \
                      $dir/example/data/*.dat \
                      $dir/include/* \
                      $dir/**/(READM*[A-Z]|INSTALL|CHANGELOG|Doxyfile)\
                      $dir/lib/.dummy \
		      --exclude .svn*

    echo ""
    # if it's gavin running this then automatically copy the tarfile
    # to the web-space
    if [[ $USER = salam ]]
    then
      echo "Copying .tgz file to web-site"
      cp -vp $tarname ~salam/www/repository/software/fastjet/
      echo "************   Remember to edit web page **********"
    fi

    rm $dir
  fi
fi

#tar zcf $tarname
popd
mv -v Makefile.orig Makefile
#echo "Putting original Makefile back"


