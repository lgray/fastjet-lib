#!/bin/zsh
# create a tar archive

#version=0.9.0c-20050929-1200
#version=0.9pre-20060203-2140
version=1.0.0b2
origdir=`pwd | sed 's/.*\///'`
echo "Will make an archive of $origdir/"
dir=fastjet-$version
tarname=$dir.tgz

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
    tar zcvhf $tarname $dir/(src|include|example|doc|.)/*.(f90|f|h|hh|alg|sh|c|cc|C|tex|eps) \
                      $dir/(src|include|example|doc)/Makefile \
                      $dir/Makefile \
                      $dir/data/*.dat \
                      $dir/**/(READM*[A-Z]|INSTALL|CHANGELOG|Doxyfile)\
                      $dir/lib/.dummy 

    echo ""
    # make sure that the Makefile does not use CGAL by default, so as
    # to make it easier for people to compile from the package.
    if [[ `grep USE_CGAL $dir/Makefile | grep -ci yes` -ne 0 ]]
    then 
      echo "WARNING: USE_CGAL is yes..."
    else
      # if it's gavin running this then automatically copy the tarfile
      # to the web-space
      if [[ $USER = salam ]]
      then
        echo "Copying .tgz file to web-site"
        cp -vp $tarname ~salam/www/repository/software/fastjet/
      fi
    fi

    rm $dir
  fi
fi

#tar zcf $tarname
popd


