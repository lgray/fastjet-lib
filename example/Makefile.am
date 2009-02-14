# for the time being allow these things to be distributed but without
# any particular help to go with them? This should change at some point

SUBDIRS = data root
EXTRA_DIST = Makefile.alt ktjet_example.cc ktjet_timing.cc CmdLine.hh\
	makefile.static
#	test-example.sh test-areas.sh makefile.static

noinst_PROGRAMS = fastjet_example\
	fastjet_areas fastjet_subtraction\
	fastjet_timing fastjet_timing_plugins fastjet_subjets fastjet_boosted_higgs

check_PROGRAMS = fastjet_example fastjet_areas

fastjet_example_SOURCES = fastjet_example.cc
fastjet_example_CXXFLAGS = ${AM_CXXFLAGS} ${CGAL_CPPFLAGS} -Woverloaded-virtual -I${srcdir}/../include
fastjet_example_LDADD = ../src/libfastjet.la ${CGAL_LIBS}

fastjet_areas_SOURCES = fastjet_areas.cc
fastjet_areas_CXXFLAGS = ${AM_CXXFLAGS} ${CGAL_CPPFLAGS} -Woverloaded-virtual -I${srcdir}/../include
fastjet_areas_LDADD = ../src/libfastjet.la ${CGAL_LIBS}
if BUILD_PLUGIN_SISCONE
fastjet_areas_CXXFLAGS += -I${srcdir}/../plugins/SISCone -I${srcdir}/../plugins/SISCone/siscone
fastjet_areas_LDADD += ../plugins/SISCone/libSISConePlugin.la ../plugins/SISCone/siscone/siscone/libsiscone.la ../plugins/SISCone/siscone/siscone/spherical/libsiscone_spherical.la
endif

fastjet_subtraction_SOURCES = fastjet_subtraction.cc
fastjet_subtraction_CXXFLAGS = ${AM_CXXFLAGS} ${CGAL_CPPFLAGS} -Woverloaded-virtual -I${srcdir}/../include
fastjet_subtraction_LDADD = ../src/libfastjet.la ${CGAL_LIBS}

fastjet_subjets_SOURCES = fastjet_subjets.cc
fastjet_subjets_CXXFLAGS = ${AM_CXXFLAGS} ${CGAL_CPPFLAGS} -Woverloaded-virtual -I${srcdir}/../include
fastjet_subjets_LDADD = ../src/libfastjet.la ${CGAL_LIBS}

fastjet_boosted_higgs_SOURCES = fastjet_boosted_higgs.cc
fastjet_boosted_higgs_CXXFLAGS = ${AM_CXXFLAGS} ${CGAL_CPPFLAGS} -Woverloaded-virtual -I${srcdir}/../include
fastjet_boosted_higgs_LDADD = ../src/libfastjet.la ${CGAL_LIBS}

fastjet_timing_SOURCES = fastjet_timing.cc CmdLine.cc
fastjet_timing_CXXFLAGS = ${AM_CXXFLAGS} ${CGAL_CPPFLAGS} -Woverloaded-virtual -I${srcdir}/../include
fastjet_timing_LDADD = ../src/libfastjet.la ${CGAL_LIBS}

fastjet_timing_plugins_SOURCES = fastjet_timing_plugins.cc CmdLine.cc
fastjet_timing_plugins_CXXFLAGS = ${AM_CXXFLAGS} ${CGAL_CPPFLAGS} -Woverloaded-virtual -I${srcdir}/../include
fastjet_timing_plugins_LDADD = ../src/libfastjet.la ${CGAL_LIBS}
if BUILD_PLUGIN_SISCONE
fastjet_timing_plugins_CXXFLAGS += -I${srcdir}/../plugins/SISCone -I${srcdir}/../plugins/SISCone/siscone
fastjet_timing_plugins_LDADD += ../plugins/SISCone/libSISConePlugin.la ../plugins/SISCone/siscone/siscone/libsiscone.la ../plugins/SISCone/siscone/siscone/spherical/libsiscone_spherical.la
endif
if BUILD_PLUGIN_CDFCONES
fastjet_timing_plugins_CXXFLAGS += -I${srcdir}/../plugins/CDFCones
fastjet_timing_plugins_LDADD += ../plugins/CDFCones/libCDFConesPlugin.la ../plugins/CDFCones/CDFcode/libCDFcode.la
endif
if BUILD_PLUGIN_PXCONE
fastjet_timing_plugins_CXXFLAGS += -I${srcdir}/../plugins/PxCone
fastjet_timing_plugins_LDADD += ../plugins/PxCone/libPxConePlugin.la ${FLIBS}
endif
if BUILD_PLUGIN_D0RUNIICONE
fastjet_timing_plugins_CXXFLAGS += -I${srcdir}/../plugins/D0RunIICone
fastjet_timing_plugins_LDADD += ../plugins/D0RunIICone/libD0RunIIConePlugin.la ${FLIBS}
endif

if BUILD_PLUGIN_TRACKJET
fastjet_timing_plugins_CXXFLAGS += -I${srcdir}/../plugins/TrackJet
fastjet_timing_plugins_LDADD += ../plugins/TrackJet/libTrackJetPlugin.la
endif

if BUILD_PLUGIN_ATLASCONE
fastjet_timing_plugins_CXXFLAGS += -I${srcdir}/../plugins/ATLASCone
fastjet_timing_plugins_LDADD += ../plugins/ATLASCone/libATLASConePlugin.la
endif

if BUILD_PLUGIN_EECAMBRIDGE
fastjet_timing_plugins_CXXFLAGS += -I${srcdir}/../plugins/EECambridge
fastjet_timing_plugins_LDADD += ../plugins/EECambridge/libEECambridgePlugin.la
endif

# add entries for other plugins here

#TESTS_ENVIRONMENT = $(SHELL)
#TESTS = test-example.sh test-areas.sh
