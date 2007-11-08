# for the time being allow these things to be distributed but without
# any particular help to go with them? This should change at some point

SUBDIRS = data root
EXTRA_DIST = Makefile.alt ktjet_example.cc ktjet_timing.cc CmdLine.hh\
	test-example.sh test-areas.sh makefile.static

noinst_PROGRAMS = fastjet_example fastjet_example_v1_interface\
	fastjet_areas fastjet_subtraction\
	fastjet_timing fastjet_timing_plugins

check_PROGRAMS = fastjet_example fastjet_areas

fastjet_example_SOURCES = fastjet_example.cc
fastjet_example_CXXFLAGS = ${AM_CXXFLAGS} ${CGAL_CPPFLAGS} -Woverloaded-virtual -I${srcdir}/../include
fastjet_example_LDADD = ../src/libfastjet.la ${CGAL_LIBS}

fastjet_example_v1_interface_SOURCES = fastjet_example_v1_interface.cc
fastjet_example_v1_interface_CXXFLAGS = ${AM_CXXFLAGS} ${CGAL_CPPFLAGS} -Woverloaded-virtual -I${srcdir}/../include
fastjet_example_v1_interface_LDADD = ../src//libfastjet.la ${CGAL_LIBS}

fastjet_areas_SOURCES = fastjet_areas.cc
fastjet_areas_CXXFLAGS = ${AM_CXXFLAGS} ${CGAL_CPPFLAGS} -Woverloaded-virtual -I${srcdir}/../include
fastjet_areas_LDADD = ../src/libfastjet.la ${CGAL_LIBS}
if BUILD_PLUGIN_SISCONE
fastjet_areas_CXXFLAGS += -I${srcdir}/../plugins/SISCone
fastjet_areas_LDADD += ../plugins/SISCone/libSISConePlugin.la ../plugins/SISCone/siscone/siscone/libsiscone.la
endif

fastjet_subtraction_SOURCES = fastjet_subtraction.cc
fastjet_subtraction_CXXFLAGS = ${AM_CXXFLAGS} ${CGAL_CPPFLAGS} -Woverloaded-virtual -I${srcdir}/../include
fastjet_subtraction_LDADD = ../src/libfastjet.la ${CGAL_LIBS}

fastjet_timing_SOURCES = fastjet_timing.cc CmdLine.cc
fastjet_timing_CXXFLAGS = ${AM_CXXFLAGS} ${CGAL_CPPFLAGS} -Woverloaded-virtual -I${srcdir}/../include
fastjet_timing_LDADD = ../src/libfastjet.la ${CGAL_LIBS}

fastjet_timing_plugins_SOURCES = fastjet_timing_plugins.cc CmdLine.cc
fastjet_timing_plugins_CXXFLAGS = ${AM_CXXFLAGS} ${CGAL_CPPFLAGS} -Woverloaded-virtual -I${srcdir}/../include
fastjet_timing_plugins_LDADD = ../src/libfastjet.la ${CGAL_LIBS}
if BUILD_PLUGIN_SISCONE
fastjet_timing_plugins_CXXFLAGS += -I${srcdir}/../plugins/SISCone
fastjet_timing_plugins_LDADD += ../plugins/SISCone/libSISConePlugin.la ../plugins/SISCone/siscone/siscone/libsiscone.la
endif
if BUILD_PLUGIN_CDFCONES
fastjet_timing_plugins_CXXFLAGS += -I${srcdir}/../plugins/CDFCones
fastjet_timing_plugins_LDADD += ../plugins/CDFCones/libCDFConesPlugin.la ../plugins/CDFCones/CDFcode/libCDFcode.la
endif
if BUILD_PLUGIN_PXCONE
fastjet_timing_plugins_CXXFLAGS += -I${srcdir}/../plugins/PxCone
fastjet_timing_plugins_LDADD += ../plugins/PxCone/libPxConePlugin.la ${FLIBS}
endif

#TESTS_ENVIRONMENT = $(SHELL)
#TESTS = test-example.sh test-areas.sh
