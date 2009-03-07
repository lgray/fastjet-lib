#EXTRA_DIST = test-plugins.sh run_jet_finder.hh makefile.static
EXTRA_DIST = run_jet_finder.hh makefile.static

noinst_PROGRAMS = many_algs_example
many_algs_example_SOURCES = run_jet_finder.cc many_algs_example.cc 
many_algs_example_CXXFLAGS = ${AM_CXXFLAGS} ${CGAL_CPPFLAGS} -Woverloaded-virtual -I${srcdir}/../../include -I../../include
many_algs_example_LDADD = ../../src/libfastjet.la ${CGAL_LIBS}

# add SISCone-related parts
if BUILD_PLUGIN_SISCONE
noinst_PROGRAMS += siscone_example
siscone_example_SOURCES = run_jet_finder.cc siscone_example.cc
siscone_example_CXXFLAGS = ${AM_CXXFLAGS} ${CGAL_CPPFLAGS} -Woverloaded-virtual -I${srcdir}/../../include  -I../../include -I${srcdir}/../SISCone -I${srcdir}/../SISCone/siscone
siscone_example_LDADD = ../../src/libfastjet.la ${CGAL_LIBS} ../SISCone/libSISConePlugin.la ../SISCone/siscone/siscone/libsiscone.la  ../SISCone/siscone/siscone/spherical/libsiscone_spherical.la 

many_algs_example_CXXFLAGS += -I${srcdir}/../SISCone -I${srcdir}/../SISCone/siscone
many_algs_example_LDADD += ../SISCone/libSISConePlugin.la ../SISCone/siscone/siscone/libsiscone.la ../SISCone/siscone/siscone/spherical/libsiscone_spherical.la
endif

# add CDFCones-related parts
if BUILD_PLUGIN_CDFCONES
noinst_PROGRAMS += cdfmidpoint_example
cdfmidpoint_example_SOURCES = run_jet_finder.cc cdfmidpoint_example.cc
cdfmidpoint_example_CXXFLAGS = ${AM_CXXFLAGS} ${CGAL_CPPFLAGS} -Woverloaded-virtual -I${srcdir}/../../include  -I../../include -I${srcdir}/../CDFCones
cdfmidpoint_example_LDADD = ../../src/libfastjet.la ${CGAL_LIBS} ../CDFCones/libCDFConesPlugin.la ../CDFCones/CDFcode/libCDFcode.la

many_algs_example_CXXFLAGS += -I${srcdir}/../CDFCones
many_algs_example_LDADD += ../CDFCones/libCDFConesPlugin.la ../CDFCones/CDFcode/libCDFcode.la
endif

# add PxCone-related parts
if BUILD_PLUGIN_PXCONE
noinst_PROGRAMS += pxcone_example
pxcone_example_SOURCES = run_jet_finder.cc pxcone_example.cc
pxcone_example_CXXFLAGS = ${AM_CXXFLAGS} ${CGAL_CPPFLAGS} -Woverloaded-virtual -I${srcdir}/../../include  -I../../include -I${srcdir}/../PxCone
pxcone_example_LDADD = ../../src/libfastjet.la ${CGAL_LIBS} ../PxCone/libPxConePlugin.la ${FLIBS}

many_algs_example_CXXFLAGS += -I${srcdir}/../PxCone
many_algs_example_LDADD += ../PxCone/libPxConePlugin.la ${FLIBS}
endif

# add D0RunIICones-related parts
if BUILD_PLUGIN_D0RUNIICONE
many_algs_example_CXXFLAGS += -I${srcdir}/../D0RunIICone
many_algs_example_LDADD += ../D0RunIICone/libD0RunIIConePlugin.la
endif

# add TrackJet-related parts
if BUILD_PLUGIN_TRACKJET
many_algs_example_CXXFLAGS += -I${srcdir}/../TrackJet
many_algs_example_LDADD += ../TrackJet/libTrackJetPlugin.la
endif

# add ATLASCone-related parts
if BUILD_PLUGIN_ATLASCONE
many_algs_example_CXXFLAGS += -I${srcdir}/../ATLASCone
many_algs_example_LDADD += ../ATLASCone/libATLASConePlugin.la
endif

# add EECambridge-related parts
if BUILD_PLUGIN_EECAMBRIDGE
many_algs_example_CXXFLAGS += -I${srcdir}/../EECambridge
many_algs_example_LDADD += ../EECambridge/libEECambridgePlugin.la
endif

# add Jade-related parts
if BUILD_PLUGIN_JADE
many_algs_example_CXXFLAGS += -I${srcdir}/../Jade
many_algs_example_LDADD += ../Jade/libJadePlugin.la
endif

# add entries for other plugins here

#TEST_ENVIRONMENT = $(SHELL)
#TESTS = test-plugins.sh
