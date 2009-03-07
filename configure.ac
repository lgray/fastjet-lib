AC_INIT([FastJet],[2.4-devel])
AC_CONFIG_SRCDIR([src/JetDefinition.cc])
AM_INIT_AUTOMAKE

dnl uncomment the following line if you want to use autoheader
AM_CONFIG_HEADER(include/fastjet/config_auto.h:config.h.in)
dnl and also uncomment the call to autoheader in autogen.sh

dnl check autoconf version
AC_PREREQ(2.57)

dnl check basic types
AC_CHECK_TYPE(int)
AC_CHECK_TYPE(long)

dnl if the user has not set CXXFLAGS, then set our own default
test "x${CXXFLAGS+yes}" == "xyes" || CXXFLAGS="-O3 -Wall"

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
dnl AM_CXXFLAGS=" -I../include "
CGAL_CPPFLAGS=""
CGAL_LIBS=""

dnl if the debug flag is set, build with -g
dnl default is "yes"
AC_ARG_ENABLE(debug,
	[  --enable-debug         Turn on debug compiler information],
	[ENABLE_DEBUG_FLAG="$enableval"],
	[ENABLE_DEBUG_FLAG="yes"])
if [[ "$ENABLE_DEBUG_FLAG" == "yes" ]] ; then
   AM_CXXFLAGS=${AM_CXXFLAGS}"-g "
fi

dnl build the shared lib by default
dnl even though they're enabled by default, AM_ENABLE_SHARED has to
dnl be called so that ${enable_shared} is set to 'yes' instead of ''
AM_ENABLE_SHARED
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
       [  --enable-cgal           enables link with the CGAL library (default=no)],
       [ENABLE_CGAL_FLAG="$enableval"],
       [ENABLE_CGAL_FLAG="no"])
dnl if asked for, check if the CGAL library can be used
if [[ "x${ENABLE_CGAL_FLAG}" == "xyes" ]] ; then
   ACX_CHECK_CGAL([],[ENABLE_CGAL_FLAG="no"])
fi

dnl update compilation/link flags for the CGAL lib
if [[ "x$ENABLE_CGAL_FLAG" == "xyes" ]] ; then
   CONFIG_CXXFLAGS=${CONFIG_CXXFLAGS}" $CGAL_CPPFLAGS "
   CONFIG_LIBS=${CONFIG_LIBS}" $CGAL_LDFLAGS $CGAL_LIBS "
else
   CONFIG_CXXFLAGS=${CONFIG_CXXFLAGS}" -DDROP_CGAL "
fi
AC_SUBST(CGAL_CPPFLAGS)
AC_SUBST(CGAL_LIBS)

dnl enable libtool
AC_PROG_LIBTOOL

dnl substitutions
AM_CXXFLAGS=${AM_CXXFLAGS}"$CONFIG_CXXFLAGS"
dnl AM_LIBS="$CONFIG_LIBS"
AC_SUBST(AM_CXXFLAGS)
AC_SUBST(CONFIG_LIBS)

dnl create the substitutions variables for the configuration script
AC_SUBST(PACKAGE_VERSION)
AC_SUBST(HAS_SHARED)

dnl----------------
dnl plugin section
dnl each plugin can be enabled or disabled (default is SISCone and CDFCones)
dnl We also define a ENABLED_PLUGIN_<name> in config.h when plugin
dnl <name> is enabled
dnl WARNING: for make dist reasons all Makefiles MUST be created
dnl----------------
dnl general
AC_CONFIG_FILES( plugins/Makefile plugins/usage_examples/Makefile )
CONFIG_LIBS_PLUGINS=""
CONFIG_LIBS_PLUGINS_STATIC=""

dnl deal with a couple of arguments to allow a whole bunch of plugins at a time

dnl default plugin settings
DEFAULT_ENABLE_SISCONE="yes"
DEFAULT_ENABLE_CDFCONES="yes"
DEFAULT_ENABLE_PXCONE="no"
DEFAULT_ENABLE_D0RUNIICONE="no"
DEFAULT_ENABLE_NESTEDDEFS="yes"
DEFAULT_ENABLE_TRACKJET="no"
DEFAULT_ENABLE_ATLASCONE="no"
DEFAULT_ENABLE_EECAMBRIDGE="yes"
DEFAULT_ENABLE_JADE="yes"

dnl Allow to enable all the plugins at a go
dnl Note: we do actually only change the default values, so that it allows for things like
dnl          ./configure --enable-allplugins --disable-pxcone
dnl       will enable everything but PxCone
dnl Note: --disable-allplugins is automatically defined by autoconf
dnl       Therefore, we implement it so as it does what it is supposed to do i.e. disable
dnl       all the plugins. Again, --disable-allplugins --enable-siscone shall work and
dnl       just enable the SISCone plugin
AC_MSG_CHECKING([whether to build all plugins])
AC_ARG_ENABLE(allplugins,
       [  --enable-allplugins     enables all the FastJet plugins (default=no)],
       [ENABLE_ALLPLUGINS="$enableval"],
       [ENABLE_ALLPLUGINS="not set"])
if [[ "x$ENABLE_ALLPLUGINS" == "xyes" ]] ; then
       DEFAULT_ENABLE_SISCONE="yes"
       DEFAULT_ENABLE_CDFCONES="yes"
       DEFAULT_ENABLE_PXCONE="yes"
       DEFAULT_ENABLE_D0RUNIICONE="yes"
       DEFAULT_ENABLE_NESTEDDEFS="yes"
       DEFAULT_ENABLE_TRACKJET="yes"
       DEFAULT_ENABLE_ATLASCONE="yes"
       DEFAULT_ENABLE_EECAMBRIDGE="yes" 
       DEFAULT_ENABLE_JADE="yes"
fi
if [[ "x$ENABLE_ALLPLUGINS" == "xno" ]] ; then
       DEFAULT_ENABLE_SISCONE="no"
       DEFAULT_ENABLE_CDFCONES="no"
       DEFAULT_ENABLE_PXCONE="no"
       DEFAULT_ENABLE_D0RUNIICONE="no"
       DEFAULT_ENABLE_NESTEDDEFS="no"
       DEFAULT_ENABLE_TRACKJET="no"
       DEFAULT_ENABLE_ATLASCONE="no"
       DEFAULT_ENABLE_EECAMBRIDGE="no" 
       DEFAULT_ENABLE_JADE="no"
fi
AC_MSG_RESULT($ENABLE_ALLPLUGINS)

dnl Allow to enable all the CXX plugins at a go
dnl Note: we do actually only change the default values, so that it allows for things like
dnl          ./configure --enable-allcxxplugins --disable-cdfcones
dnl       will enable everything but PxCone
dnl Note: --disable-allplugins is automatically defined by autoconf
dnl       Therefore, we implement it so as it does what it is supposed to do i.e. disable
dnl       all the plugins. Again, --disable-allcxxplugins --enable-siscone shall work and
dnl       just enable the SISCone plugin
AC_MSG_CHECKING([whether to build all CXX plugins])
AC_ARG_ENABLE(allcxxplugins,
       [  --enable-allcxxplugins  enables all the CXX FastJet plugins (default=no)],
       [ENABLE_ALLCXXPLUGINS="$enableval"],
       [ENABLE_ALLCXXPLUGINS="not set"])
if [[ "x$ENABLE_ALLCXXPLUGINS" == "xyes" ]] ; then
       DEFAULT_ENABLE_SISCONE="yes"
       DEFAULT_ENABLE_CDFCONES="yes"
       DEFAULT_ENABLE_D0RUNIICONE="yes"
       DEFAULT_ENABLE_NESTEDDEFS="yes"
       DEFAULT_ENABLE_TRACKJET="yes"
       DEFAULT_ENABLE_ATLASCONE="yes"
       DEFAULT_ENABLE_EECAMBRIDGE="yes" 
       DEFAULT_ENABLE_JADE="yes"
fi
if [[ "x$ENABLE_ALLCXXPLUGINS" == "xno" ]] ; then
       DEFAULT_ENABLE_SISCONE="no"
       DEFAULT_ENABLE_CDFCONES="no"
       DEFAULT_ENABLE_D0RUNIICONE="no"
       DEFAULT_ENABLE_NESTEDDEFS="no"
       DEFAULT_ENABLE_TRACKJET="no"
       DEFAULT_ENABLE_ATLASCONE="no"
       DEFAULT_ENABLE_EECAMBRIDGE="no" 
       DEFAULT_ENABLE_JADE="no"
fi
AC_MSG_RESULT($ENABLE_ALLCXXPLUGINS)


dnl SISCone
AC_MSG_CHECKING([whether to build plugin SISCone])
AC_ARG_ENABLE(siscone,
       [  --enable-siscone        enables the SISCone plugin (default=yes)],
       [ENABLE_SISCONE="$enableval"],
       [ENABLE_SISCONE="$DEFAULT_ENABLE_SISCONE"])
if [[ "x$ENABLE_SISCONE" == "xyes" ]] ; then
   AC_DEFINE(ENABLE_PLUGIN_SISCONE, [], [The SISCone plugin is enabled])
   CONFIG_LIBS_PLUGINS=${CONFIG_LIBS_PLUGINS}" -lSISConePlugin -lsiscone -lsiscone_spherical"
   CONFIG_LIBS_PLUGINS_STATIC=${CONFIG_LIBS_PLUGINS_STATIC}" \${installationdir}/lib/libSISConePlugin.a \${installationdir}/lib/libsiscone.a \${installationdir}/lib/libsiscone_spherical.a"
fi
AC_CONFIG_SUBDIRS(plugins/SISCone/siscone)
AC_CONFIG_FILES( plugins/SISCone/Makefile plugins/SISCone/fastjet/Makefile )
AM_CONDITIONAL(BUILD_PLUGIN_SISCONE, test x$ENABLE_SISCONE = xyes)
AC_MSG_RESULT($ENABLE_SISCONE)

dnl CDFCones
AC_MSG_CHECKING([whether to build plugin CDFCones])
AC_ARG_ENABLE(cdfcones,
       [  --enable-cdfcones       enables the CDFJetClu and CDFMidPoint plugins (default=yes)],
       [ENABLE_CDFCONES="$enableval"],
       [ENABLE_CDFCONES="$DEFAULT_ENABLE_CDFCONES"])
if [[ "x$ENABLE_CDFCONES" == "xyes" ]] ; then
   AC_DEFINE(ENABLE_PLUGIN_CDFCONES, [], [The CDFJetClu and CDFMidPoint plugins are enabled])
   CONFIG_LIBS_PLUGINS=${CONFIG_LIBS_PLUGINS}" -lCDFConesPlugin"
   CONFIG_LIBS_PLUGINS_STATIC=${CONFIG_LIBS_PLUGINS_STATIC}" \${installationdir}/lib/libCDFConesPlugin.a"
fi
AC_CONFIG_FILES(plugins/CDFCones/Makefile plugins/CDFCones/CDFcode/Makefile plugins/CDFCones/fastjet/Makefile )
AM_CONDITIONAL(BUILD_PLUGIN_CDFCONES, test x$ENABLE_CDFCONES = xyes)
AC_MSG_RESULT($ENABLE_CDFCONES)

dnl PxCone
AC_MSG_CHECKING([whether to build plugin PxCone])
AC_ARG_ENABLE(pxcone,
       [  --enable-pxcone         enables the PxCone plugin (default=no)],
       [ENABLE_PXCONE="$enableval"],
       [ENABLE_PXCONE="$DEFAULT_ENABLE_PXCONE"])
if [[ "x$ENABLE_PXCONE" == "xyes" ]] ; then
   AC_DEFINE(ENABLE_PLUGIN_PXCONE, [], [The PxCone plugin is enabled])
   dnl set up the fortran compiler and libs
   AC_PROG_F77
   AC_F77_LIBRARY_LDFLAGS
   CONFIG_LIBS_PLUGINS=${CONFIG_LIBS_PLUGINS}" -lPxConePlugin "${FLIBS}
   CONFIG_LIBS_PLUGINS_STATIC=${CONFIG_LIBS_PLUGINS_STATIC}" \${installationdir}/lib/libPxConePlugin.a "${FLIBS}
fi
AC_CONFIG_FILES( plugins/PxCone/Makefile plugins/PxCone/fastjet/Makefile )
AM_CONDITIONAL(BUILD_PLUGIN_PXCONE, test x$ENABLE_PXCONE = xyes)
AC_MSG_RESULT($ENABLE_PXCONE)

dnl D0RunIICone
AC_MSG_CHECKING([whether to build plugin D0RunIICone])
AC_ARG_ENABLE(d0runiicone,
       [  --enable-d0runiicone    enables the D0RunIICone plugin (default=no)],
       [ENABLE_D0RUNIICONE="$enableval"],
       [ENABLE_D0RUNIICONE="$DEFAULT_ENABLE_D0RUNIICONE"])
if [[ "x$ENABLE_D0RUNIICONE" == "xyes" ]] ; then
   AC_DEFINE(ENABLE_PLUGIN_D0RUNIICONE, [], [The D0RunIICone plugin is enabled])
   dnl set up the libs
   CONFIG_LIBS_PLUGINS=${CONFIG_LIBS_PLUGINS}" -lD0RunIIConePlugin "
   CONFIG_LIBS_PLUGINS_STATIC=${CONFIG_LIBS_PLUGINS_STATIC}" \${installationdir}/lib/libD0RunIIConePlugin.a "
fi
AC_CONFIG_FILES( plugins/D0RunIICone/Makefile plugins/D0RunIICone/fastjet/Makefile )
AM_CONDITIONAL(BUILD_PLUGIN_D0RUNIICONE, test x$ENABLE_D0RUNIICONE = xyes)
AC_MSG_RESULT($ENABLE_D0RUNIICONE)

dnl NestedDefs
AC_MSG_CHECKING([whether to build plugin NestedDefs])
AC_ARG_ENABLE(nesteddefs,
       [  --enable-nesteddefs     enables the NestedDefs plugin (default=yes)],
       [ENABLE_NESTEDDEFS="$enableval"],
       [ENABLE_NESTEDDEFS="$DEFAULT_ENABLE_NESTEDDEFS"])
if [[ "x$ENABLE_NESTEDDEFS" == "xyes" ]] ; then
   AC_DEFINE(ENABLE_PLUGIN_NESTEDDEFS, [], [The NestedDefs plugin is enabled])
   dnl set up the libs
   CONFIG_LIBS_PLUGINS=${CONFIG_LIBS_PLUGINS}" -lNestedDefsPlugin "
   CONFIG_LIBS_PLUGINS_STATIC=${CONFIG_LIBS_PLUGINS_STATIC}" \${installationdir}/lib/libNestedDefsPlugin.a "
fi
AC_CONFIG_FILES( plugins/NestedDefs/Makefile plugins/NestedDefs/fastjet/Makefile )
AM_CONDITIONAL(BUILD_PLUGIN_NESTEDDEFS, test x$ENABLE_NESTEDDEFS = xyes)
AC_MSG_RESULT($ENABLE_NESTEDDEFS)

dnl TrackJet
AC_MSG_CHECKING([whether to build plugin TrackJet])
AC_ARG_ENABLE(trackjet,
       [  --enable-trackjet       enables the TrackJet plugin (default=no)],
       [ENABLE_TRACKJET="$enableval"],
       [ENABLE_TRACKJET="$DEFAULT_ENABLE_TRACKJET"])
if [[ "x$ENABLE_TRACKJET" == "xyes" ]] ; then
   AC_DEFINE(ENABLE_PLUGIN_TRACKJET, [], [The TrackJet plugin is enabled])
   dnl set up the libs
   CONFIG_LIBS_PLUGINS=${CONFIG_LIBS_PLUGINS}" -lTrackJetPlugin "
   CONFIG_LIBS_PLUGINS_STATIC=${CONFIG_LIBS_PLUGINS_STATIC}" \${installationdir}/lib/libTrackJetPlugin.a "
fi
AC_CONFIG_FILES( plugins/TrackJet/Makefile plugins/TrackJet/fastjet/Makefile )
AM_CONDITIONAL(BUILD_PLUGIN_TRACKJET, test x$ENABLE_TRACKJET = xyes)
AC_MSG_RESULT($ENABLE_TRACKJET)

dnl ATLASCone
AC_MSG_CHECKING([whether to build plugin ATLASCone])
AC_ARG_ENABLE(atlascone,
       [  --enable-atlascone       enables the ATLASCone plugin (default=no)],
       [ENABLE_ATLASCONE="$enableval"],
       [ENABLE_ATLASCONE="$DEFAULT_ENABLE_ATLASCONE"])
if [[ "x$ENABLE_ATLASCONE" == "xyes" ]] ; then
   AC_DEFINE(ENABLE_PLUGIN_ATLASCONE, [], [The ATLASCone plugin is enabled])
   dnl set up the libs
   CONFIG_LIBS_PLUGINS=${CONFIG_LIBS_PLUGINS}" -lATLASConePlugin "
   CONFIG_LIBS_PLUGINS_STATIC=${CONFIG_LIBS_PLUGINS_STATIC}" \${installationdir}/lib/libATLASConePlugin.a "
fi
AC_CONFIG_FILES( plugins/ATLASCone/Makefile plugins/ATLASCone/fastjet/Makefile )
AM_CONDITIONAL(BUILD_PLUGIN_ATLASCONE, test x$ENABLE_ATLASCONE = xyes)
AC_MSG_RESULT($ENABLE_ATLASCONE)

dnl EECambridge
AC_MSG_CHECKING([whether to build plugin EECambridge])
AC_ARG_ENABLE(eecambridge,
       [  --enable-eecambridge       enables the EECambridge plugin (default=yes)],
       [ENABLE_EECAMBRIDGE="$enableval"],
       [ENABLE_EECAMBRIDGE="$DEFAULT_ENABLE_EECAMBRIDGE"])
if [[ "x$ENABLE_EECAMBRIDGE" == "xyes" ]] ; then
   AC_DEFINE(ENABLE_PLUGIN_EECAMBRIDGE, [], [The EECambridge plugin is enabled])
   dnl set up the libs
   CONFIG_LIBS_PLUGINS=${CONFIG_LIBS_PLUGINS}" -lEECambridgePlugin "
   CONFIG_LIBS_PLUGINS_STATIC=${CONFIG_LIBS_PLUGINS_STATIC}" \${installationdir}/lib/libEECambridgePlugin.a "
fi
AC_CONFIG_FILES( plugins/EECambridge/Makefile plugins/EECambridge/fastjet/Makefile )
AM_CONDITIONAL(BUILD_PLUGIN_EECAMBRIDGE, test x$ENABLE_EECAMBRIDGE = xyes)
AC_MSG_RESULT($ENABLE_EECAMBRIDGE)

dnl Jade
AC_MSG_CHECKING([whether to build plugin Jade])
AC_ARG_ENABLE(jade,
       [  --enable-jade       enables the Jade plugin (default=yes)],
       [ENABLE_JADE="$enableval"],
       [ENABLE_JADE="$DEFAULT_ENABLE_JADE"])
if [[ "x$ENABLE_JADE" == "xyes" ]] ; then
   AC_DEFINE(ENABLE_PLUGIN_JADE, [], [The Jade plugin is enabled])
   dnl set up the libs
   CONFIG_LIBS_PLUGINS=${CONFIG_LIBS_PLUGINS}" -lJadePlugin "
   CONFIG_LIBS_PLUGINS_STATIC=${CONFIG_LIBS_PLUGINS_STATIC}" \${installationdir}/lib/libJadePlugin.a "
fi
AC_CONFIG_FILES( plugins/Jade/Makefile plugins/Jade/fastjet/Makefile )
AM_CONDITIONAL(BUILD_PLUGIN_JADE, test x$ENABLE_JADE = xyes)
AC_MSG_RESULT($ENABLE_JADE)

dnl end of plugin section --- don't modify this line

AC_SUBST(CONFIG_LIBS_PLUGINS)
AC_SUBST(CONFIG_LIBS_PLUGINS_STATIC)

dnl---------------------------------
dnl make sure examples dir is there
dnl---------------------------------
AC_CONFIG_FILES( example/Makefile example/data/Makefile example/root/Makefile )

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

AC_CONFIG_FILES( Makefile src/Makefile include/Makefile include/fastjet/Makefile include/fastjet/internal/Makefile )

AC_OUTPUT

echo "---------------------------------------------------"
echo "        IMPORTANT NOTICE TO FASTJET USERS          "
echo "                                                   "
echo " When using the FastJet library, you can retrieve  "
echo " compilation and link flags using fastjet-config.  "
echo " Type 'fastjet-config --help' for more information "
echo "---------------------------------------------------"
