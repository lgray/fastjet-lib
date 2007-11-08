AC_INIT([FastJet],[2.2beta1-autoconf-devel])
AC_CONFIG_SRCDIR([src/JetDefinition.cc])
AM_INIT_AUTOMAKE

dnl uncomment the following line if you want to use autoheader
AM_CONFIG_HEADER(include/fastjet/config.h:config.h.in)
dnl and also uncomment the call to autoheader in autogen.sh

dnl check autoconf version
AC_PREREQ(2.57)

dnl check basic types
AC_CHECK_TYPE(int)
AC_CHECK_TYPE(long)

dnl check useful programs
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

dnl check standard C headers
AC_STDC_HEADERS

dnl set the default destination directory
AC_PREFIX_DEFAULT(/usr/local)

dnl set default compilation and link flags
dnl note that those can be changed by the user at configure-time
dnl so we use CXXFLAGS and not AM_CXXFLAGS
CXXFLAGS=" -O3 -Wall -I../include "
CGAL_CPPFLAGS=""
CGAL_LIBS=""

dnl if the debug flag is set, build with -g
dnl default is "yes"
AC_ARG_ENABLE(debug, 
	[  --enable-debug      Turn on debug compiler information],
	[ENABLE_DEBUG_FLAG="$enableval"],
	[ENABLE_DEBUG_FLAG="yes"])
if [[ "$ENABLE_DEBUG_FLAG" == "yes" ]] ; then
   CXXFLAGS=${CXXFLAGS}" -g "
fi

dnl don't build the shared lib by default
AM_DISABLE_SHARED
HAS_SHARED=${enable_shared}

dnl-----------------
dnl check libraries
dnl ----------------

dnl math lib
AC_CHECK_LIB(m, cos)
CONFIG_LIBS=" -lm "

dnl deal with the CGAL library
dnl first check if the user wants to use CGAL
dnl default value is "no"
dnl Note that CGAL compilation flags are considered as required
dnl so we do store them in "AM_CXXFLAGS"
CONFIG_CXXFLAGS=""
AC_ARG_ENABLE(cgal,
	[  --enable-cgal    enables link with the CGAL library (default=no)],
	[ENABLE_CGAL_FLAG="$enableval"],
	[ENABLE_CGAL_FLAG="no"])
dnl if asked for, check if the CGAL library can be used
if [[ "$ENABLE_CGAL_FLAG" == "yes" ]] ; then
   ACX_CHECK_CGAL
fi

dnl update compilation/link flags for the CGAL lib
if [[ "x$ENABLE_CGAL_FLAG" == "xyes" ]] ; then
   CONFIG_CXXFLAGS=${CONFIG_CXXFLAGS}" $CGAL_CPPFLAGS "
   CONFIG_LIBS=${CONFIG_LIBS}" $CGAL_LIBS "
else
   CONFIG_CXXFLAGS=${CONFIG_CXXFLAGS}" -DDROP_CGAL "
fi
AC_SUBST(CGAL_CPPFLAGS)
AC_SUBST(CGAL_LIBS)

dnl enable 
AC_PROG_LIBTOOL

dnl substitutions
AM_CXXFLAGS="$CONFIG_CXXFLAGS"
dnl AM_LIBS="$CONFIG_LIBS"
AC_SUBST(AM_CXXFLAGS)
AC_SUBST(CONFIG_LIBS)

dnl create the substitutions variables for the configuration script
AC_SUBST(PACKAGE_VERSION)
AC_SUBST(HAS_SHARED)

dnl----------------
dnl plugins output
dnl each plugin can be enabled or disabled (default is SISCone and CDFCones)
dnl We also define a ENABLED_PLUGIN_<name> in config.h when plugin
dnl <name> is enabled
dnl WARNING: for make dist reasons all Makefiles MUST be created
dnl----------------
dnl general
AC_CONFIG_FILES( plugins/Makefile plugins/usage_examples/Makefile )
CONFIG_LIBS_PLUGINS=""

dnl SISCone
AC_ARG_ENABLE(siscone,
	[  --enable-siscone       enables the SISCone plugin (default=yes)],
	[ENABLE_SISCONE="$enableval"],
	[ENABLE_SISCONE="yes"])
if [[ "x$ENABLE_SISCONE" == "xyes" ]] ; then
   AC_DEFINE(ENABLE_PLUGIN_SISCONE, [], [The SISCone plugin is enabled])
   CONFIG_LIBS_PLUGINS=${CONFIG_LIBS_PLUGINS}" -lSISConePlugin -lsiscone"
fi
AC_CONFIG_SUBDIRS(plugins/SISCone/siscone)
AC_CONFIG_FILES( plugins/SISCone/Makefile plugins/SISCone/fastjet/Makefile )
AM_CONDITIONAL(BUILD_PLUGIN_SISCONE, test x$ENABLE_SISCONE = xyes)

dnl CDFCones
AC_ARG_ENABLE(cdfcones,
	[  --enable-cdfcones      enables the CDFJetClu and CDFMidPoint plugins (default=yes)],
	[ENABLE_CDFCONES="$enableval"],
	[ENABLE_CDFCONES="yes"])
if [[ "x$ENABLE_CDFCONES" == "xyes" ]] ; then
   AC_DEFINE(ENABLE_PLUGIN_CDFCONES, [], [The CDFJetClu and CDFMidPoint plugins areenabled])
   CONFIG_LIBS_PLUGINS=${CONFIG_LIBS_PLUGINS}" -lCDFConesPlugin"
fi
AC_CONFIG_FILES(plugins/CDFCones/Makefile plugins/CDFCones/CDFcode/Makefile\
                plugins/CDFCones/fastjet/Makefile )
AM_CONDITIONAL(BUILD_PLUGIN_CDFCONES, test x$ENABLE_CDFCONES = xyes)

dnl PxCone
AC_ARG_ENABLE(pxcone,
	[  --enable-pxcone        enables the PxCone plugin (default=no)],
	[ENABLE_PXCONE="$enableval"],
	[ENABLE_PXCONE="no"])
if [[ "x$ENABLE_PXCONE" == "xyes" ]] ; then
   AC_DEFINE(ENABLE_PLUGIN_PXCONE, [], [The PxCone plugin is enabled])
   dnl set up the fortran compiler and libs
   AC_PROG_F77
   AC_F77_LIBRARY_LDFLAGS
   CONFIG_LIBS_PLUGINS=${CONFIG_LIBS_PLUGINS}" -lPxConePlugin "${FLIBS}
fi
AC_CONFIG_FILES( plugins/PxCone/Makefile plugins/PxCone/fastjet/Makefile )
AM_CONDITIONAL(BUILD_PLUGIN_PXCONE, test x$ENABLE_PXCONE = xyes)

AC_SUBST(CONFIG_LIBS_PLUGINS)

dnl---------------------------------
dnl make sure examples dir is there
dnl---------------------------------
AC_CONFIG_FILES( example/Makefile example/data/Makefile )

dnl---------------------------------
dnl and the documentation
dnl---------------------------------
AC_CONFIG_FILES( doc/Makefile )

dnl----------------------
dnl configuration script
dnl----------------------

AC_CONFIG_FILES([fastjet-config:fastjet-config.in], [chmod u+x fastjet-config])

dnl-----------------------
dnl sample fortran wrapper
dnl-----------------------
AC_CONFIG_FILES( fortran_wrapper/Makefile )


dnl----------------------
dnl main library outputs
dnl----------------------

AC_CONFIG_FILES( Makefile src/Makefile include/Makefile \
		 include/fastjet/Makefile include/fastjet/internal/Makefile )

AC_OUTPUT

echo "---------------------------------------------------"
echo "        IMPORTANT NOTICE TO FASTJET USERS          "
echo "                                                   "
echo " When using the FastJet library, you can retrieve  "
echo " compilation and link flags using fastjet-config.  "
echo " Type 'fastjet-config --help' for more information "
echo "---------------------------------------------------"


