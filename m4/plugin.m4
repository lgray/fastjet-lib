dnl INIT_PLUGINS
dnl initialise the default plugin behaviours
AC_DEFUN([ACX_INIT_PLUGINS],
[
    dnl default plugin settings
    DEFAULT_ENABLE_SISCONE="yes"
    DEFAULT_ENABLE_CDFCONES="yes"
    DEFAULT_ENABLE_PXCONE="no"
    DEFAULT_ENABLE_D0RUNIICONE="no"
    DEFAULT_ENABLE_NESTEDDEFS="yes"
    DEFAULT_ENABLE_TRACKJET="no"
    DEFAULT_ENABLE_ATLASCONE="no"
    DEFAULT_ENABLE_CMSITERATIVECONE="no"
    DEFAULT_ENABLE_EECAMBRIDGE="yes"
    DEFAULT_ENABLE_JADE="yes"
    DEFAULT_ENABLE_D0RUNICONE="no"

    AH_TEMPLATE(ENABLE_PLUGIN_SISCONE,          [checks if the SISCone plugin is enabled])
    AH_TEMPLATE(ENABLE_PLUGIN_CDFCONES,         [checks if the CDFCones plugin is enabled])
    AH_TEMPLATE(ENABLE_PLUGIN_PXCONE,           [checks if the PxCone plugin is enabled])
    AH_TEMPLATE(ENABLE_PLUGIN_D0RUNIICONE,      [checks if the D0RunIICone plugin is enabled])
    AH_TEMPLATE(ENABLE_PLUGIN_NESTEDDEFS,       [checks if the NestedDefs plugin is enabled])
    AH_TEMPLATE(ENABLE_PLUGIN_TRACKJET,         [checks if the TrackJet plugin is enabled])
    AH_TEMPLATE(ENABLE_PLUGIN_ATLASCONE,        [checks if the ATLASCone plugin is enabled])
    AH_TEMPLATE(ENABLE_PLUGIN_CMSITERATIVECONE, [checks if the CMSIterativeCone plugin is enabled])
    AH_TEMPLATE(ENABLE_PLUGIN_EECAMBRIDGE,      [checks if the EECambridge plugin is enabled])
    AH_TEMPLATE(ENABLE_PLUGIN_JADE,             [checks if the Jade plugin is enabled])
    AH_TEMPLATE(ENABLE_PLUGIN_D0RUNICONE,       [checks if the D0RunICone plugin is enabled])
])


dnl Allow to enable all the plugins at a go
dnl Note: we do actually only change the default values, so that it allows for things like
dnl          ./configure --enable-allplugins --disable-pxcone
dnl       will enable everything but PxCone
dnl Note: --disable-allplugins is automatically defined by autoconf
dnl       Therefore, we implement it so as it does what it is supposed to do i.e. disable
dnl       all the plugins. Again, --disable-allplugins --enable-siscone shall work and
dnl       just enable the SISCone plugin
AC_DEFUN([ACX_CHECK_ALLPLUGINS],
[
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
    	DEFAULT_ENABLE_CMSITERATIVECONE="yes"
    	DEFAULT_ENABLE_EECAMBRIDGE="yes" 
    	DEFAULT_ENABLE_JADE="yes"
	DEFAULT_ENABLE_D0RUNICONE="yes"
    fi
    if [[ "x$ENABLE_ALLPLUGINS" == "xno" ]] ; then
    	DEFAULT_ENABLE_SISCONE="no"
    	DEFAULT_ENABLE_CDFCONES="no"
    	DEFAULT_ENABLE_PXCONE="no"
    	DEFAULT_ENABLE_D0RUNIICONE="no"
    	DEFAULT_ENABLE_NESTEDDEFS="no"
    	DEFAULT_ENABLE_TRACKJET="no"
    	DEFAULT_ENABLE_ATLASCONE="no"
    	DEFAULT_ENABLE_CMSITERATIVECONE="no"
    	DEFAULT_ENABLE_EECAMBRIDGE="no" 
    	DEFAULT_ENABLE_JADE="no"
	DEFAULT_ENABLE_D0RUNICONE="no"
    fi
    AC_MSG_RESULT($ENABLE_ALLPLUGINS)
])


dnl Allow to enable all the CXX plugins at a go
dnl Note: we do actually only change the default values, so that it allows for things like
dnl          ./configure --enable-allcxxplugins --disable-cdfcones
dnl       will enable everything but PxCone
dnl Note: --disable-allplugins is automatically defined by autoconf
dnl       Therefore, we implement it so as it does what it is supposed to do i.e. disable
dnl       all the plugins. Again, --disable-allcxxplugins --enable-siscone shall work and
dnl       just enable the SISCone plugin
AC_DEFUN([ACX_CHECK_ALLCXXPLUGINS],
[
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
    	DEFAULT_ENABLE_CMSITERATIVECONE="yes"
    	DEFAULT_ENABLE_EECAMBRIDGE="yes" 
    	DEFAULT_ENABLE_JADE="yes"
	DEFAULT_ENABLE_D0RUNICONE="yes"
    fi
    if [[ "x$ENABLE_ALLCXXPLUGINS" == "xno" ]] ; then
    	DEFAULT_ENABLE_SISCONE="no"
    	DEFAULT_ENABLE_CDFCONES="no"
    	DEFAULT_ENABLE_D0RUNIICONE="no"
    	DEFAULT_ENABLE_NESTEDDEFS="no"
    	DEFAULT_ENABLE_TRACKJET="no"
    	DEFAULT_ENABLE_ATLASCONE="no"
    	DEFAULT_ENABLE_CMSITERATIVECONE="no"
    	DEFAULT_ENABLE_EECAMBRIDGE="no" 
    	DEFAULT_ENABLE_JADE="no"
	DEFAULT_ENABLE_D0RUNICONE="no"
    fi
    AC_MSG_RESULT($ENABLE_ALLCXXPLUGINS)
])



dnl CHECK PLUGIN
dnl This script is meant to check if a plugin has to be enabled
dnl It takes the following arguments
dnl   the name of the plugin (e.g. SISCone)
dnl   the name in lower case (e.g. siscone)
dnl   the name in upper case (e.g. SISCONE)
dnl   commands to execute if enabled
dnl   commands to execute if disabled
dnl it sets the following vars
dnl 
AC_DEFUN([ACX_CHECK_PLUGIN],
[
    AC_MSG_CHECKING([whether to build plugin $1])

    AC_ARG_ENABLE($2,
	[  --enable-$2       enables the $1 plugin [[default=$4]]],
	[ENABLE_$3="$enableval"],
	[ENABLE_$3="${DEFAULT_ENABLE_$3}"])
    if [[ "x$ENABLE_$3" == "xyes" ]] ; then
        AC_DEFINE(ENABLE_PLUGIN_$3, [], [The $1 plugin is enabled])
   	dnl set up the libs
	if [[ "x$monolithic" != "xtrue" ]] ; then
   	    CONFIG_LIBS_PLUGINS=${CONFIG_LIBS_PLUGINS}" -l$1Plugin "
   	    CONFIG_LIBS_PLUGINS_STATIC=${CONFIG_LIBS_PLUGINS_STATIC}" \${installationdir}/lib/lib$1Plugin.a "
	fi

	dnl check if there is some provided commands to execute
   	ifelse([$5],[],[:],[$5])

	LIST_ALL_PLUGINS=${LIST_ALL_PLUGINS}" $1"
    else
	ifelse([$6],[],[:],[$6])   
    fi

    AC_CONFIG_FILES( plugins/$1/Makefile plugins/$1/fastjet/Makefile )
    AM_CONDITIONAL(BUILD_PLUGIN_$3, test x$ENABLE_$3 = xyes)

    AC_MSG_RESULT(${ENABLE_$3})
])
