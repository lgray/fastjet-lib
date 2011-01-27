# for the time being allow these things to be distributed but without
# any particular help to go with them? This should change at some point

SUBDIRS = data root
EXTRA_DIST = Makefile.alt ktjet_example.cc ktjet_timing.cc CmdLine.hh\
	makefile.static
#	test-example.sh test-areas.sh makefile.static

noinst_PROGRAMS = fastjet_example fastjet_areas fastjet_timing_plugins\
 01-basic  \
 02-jetdef \
 04-constituents \
 05-eplus_eminus \
 06-area \
 07-subtraction \
 08-selector \
 09-extra_info \
 10-subjets \
 11-boosted_higgs

check_PROGRAMS = fastjet_example fastjet_areas

# basic usage example
01_basic_SOURCES = 01-basic.cc
01_basic_SHORTNAME = 01
01_basic_CXXFLAGS = $(AM_CXXFLAGS) $(CGAL_CPPFLAGS) -I$(srcdir)/../include
01_basic_LDADD = ../src/libfastjet.la $(CGAL_LIBS)

# further illustrates the usage of jet definitions
02_jetdef_SOURCES = 02-jetdef.cc
02_jetdef_SHORTNAME = 02
02_jetdef_CXXFLAGS = $(AM_CXXFLAGS) $(CGAL_CPPFLAGS) -I$(srcdir)/../include
02_jetdef_LDADD = ../src/libfastjet.la $(CGAL_LIBS)

# illustrates the usage of plugins
if BUILD_PLUGIN_SISCONE
noinst_PROGRAMS += 03-plugin
03_plugin_SOURCES = 03-plugin.cc
#03_plugin_SHORTNAME = 03
03_plugin_CXXFLAGS = $(AM_CXXFLAGS) $(CGAL_CPPFLAGS) -I$(srcdir)/../include
03_plugin_LDADD = ../src/libfastjet.la $(CGAL_LIBS)
03_plugin_CXXFLAGS += -I$(srcdir)/../plugins/SISCone -I$(srcdir)/../plugins/SISCone/siscone
03_plugin_LDADD += ../plugins/SISCone/libSISConePlugin.la ../plugins/SISCone/siscone/siscone/libsiscone.la ../plugins/SISCone/siscone/siscone/spherical/libsiscone_spherical.la
else
EXTRA_DIST += 03-plugin.cc
endif

# illustrate the access to the constituents
04_constituents_SOURCES = 04-constituents.cc
04_constituents_SHORTNAME = 04
04_constituents_CXXFLAGS = $(AM_CXXFLAGS) $(CGAL_CPPFLAGS) -I$(srcdir)/../include
04_constituents_LDADD = ../src/libfastjet.la $(CGAL_LIBS)

# illustrate the access to the constituents
05_eplus_eminus_SOURCES = 05-eplus_eminus.cc
05_eplus_eminus_SHORTNAME = 05
05_eplus_eminus_CXXFLAGS = $(AM_CXXFLAGS) $(CGAL_CPPFLAGS) -I$(srcdir)/../include
05_eplus_eminus_LDADD = ../src/libfastjet.la $(CGAL_LIBS)

# illustrate the access to the constituents
06_area_SOURCES = 06-area.cc
06_area_SHORTNAME = 06
06_area_CXXFLAGS = $(AM_CXXFLAGS) $(CGAL_CPPFLAGS) -I$(srcdir)/../include
06_area_LDADD = ../src/libfastjet.la $(CGAL_LIBS)

# illustrate the access to the constituents
07_subtraction_SOURCES = 07-subtraction.cc
07_subtraction_SHORTNAME = 07
07_subtraction_CXXFLAGS = $(AM_CXXFLAGS) $(CGAL_CPPFLAGS) -I$(srcdir)/../include
07_subtraction_LDADD = ../src/libfastjet.la $(CGAL_LIBS)

# illustrate the access to the constituents
08_selector_SOURCES = 08-selector.cc
08_selector_SHORTNAME = 08
08_selector_CXXFLAGS = $(AM_CXXFLAGS) $(CGAL_CPPFLAGS) -I$(srcdir)/../include -I$(srcdir)/../tools
08_selector_LDADD = ../src/libfastjet.la ../tools/libfastjettools.la $(CGAL_LIBS)

# illustrate the access to the constituents
09_extra_info_SOURCES = 09-extra_info.cc
09_extra_info_SHORTNAME = 09
09_extra_info_CXXFLAGS = $(AM_CXXFLAGS) $(CGAL_CPPFLAGS) -I$(srcdir)/../include -I$(srcdir)/../tools
09_extra_info_LDADD = ../src/libfastjet.la ../tools/libfastjettools.la $(CGAL_LIBS)

# illustrate the access to the constituents
10_subjets_SOURCES = 10-subjets.cc
10_subjets_SHORTNAME = 10
10_subjets_CXXFLAGS = $(AM_CXXFLAGS) $(CGAL_CPPFLAGS) -I$(srcdir)/../include -I$(srcdir)/../tools
10_subjets_LDADD = ../src/libfastjet.la $(CGAL_LIBS)

# illustrate the access to the constituents
11_boosted_higgs_SOURCES = 11-boosted_higgs.cc
11_boosted_higgs_SHORTNAME = 11
11_boosted_higgs_CXXFLAGS = $(AM_CXXFLAGS) $(CGAL_CPPFLAGS) -I$(srcdir)/../include -I$(srcdir)/../tools
11_boosted_higgs_LDADD = ../src/libfastjet.la $(CGAL_LIBS)



# backward-compatible + make-check-related things
fastjet_example_SOURCES = fastjet_example.cc
fastjet_example_CXXFLAGS = $(AM_CXXFLAGS) $(CGAL_CPPFLAGS) -I$(srcdir)/../include
fastjet_example_LDADD = ../src/libfastjet.la $(CGAL_LIBS)

fastjet_areas_SOURCES = fastjet_areas.cc
fastjet_areas_CXXFLAGS = $(AM_CXXFLAGS) $(CGAL_CPPFLAGS) -I$(srcdir)/../include
fastjet_areas_LDADD = ../src/libfastjet.la $(CGAL_LIBS)
if BUILD_PLUGIN_SISCONE
fastjet_areas_CXXFLAGS += -I$(srcdir)/../plugins/SISCone -I$(srcdir)/../plugins/SISCone/siscone
fastjet_areas_LDADD += ../plugins/SISCone/libSISConePlugin.la ../plugins/SISCone/siscone/siscone/libsiscone.la ../plugins/SISCone/siscone/siscone/spherical/libsiscone_spherical.la
endif

fastjet_timing_plugins_SOURCES = fastjet_timing_plugins.cc CmdLine.cc
fastjet_timing_plugins_CXXFLAGS = $(AM_CXXFLAGS) $(CGAL_CPPFLAGS) -I$(srcdir)/../include
fastjet_timing_plugins_LDADD = ../src/libfastjet.la $(CGAL_LIBS)
if BUILD_PLUGIN_SISCONE
fastjet_timing_plugins_CXXFLAGS += -I$(srcdir)/../plugins/SISCone -I$(srcdir)/../plugins/SISCone/siscone
fastjet_timing_plugins_LDADD += ../plugins/SISCone/libSISConePlugin.la ../plugins/SISCone/siscone/siscone/libsiscone.la ../plugins/SISCone/siscone/siscone/spherical/libsiscone_spherical.la
endif
if BUILD_PLUGIN_CDFCONES
fastjet_timing_plugins_CXXFLAGS += -I$(srcdir)/../plugins/CDFCones
fastjet_timing_plugins_LDADD += ../plugins/CDFCones/libCDFConesPlugin.la ../plugins/CDFCones/CDFcode/libCDFcode.la
endif
if BUILD_PLUGIN_PXCONE
fastjet_timing_plugins_CXXFLAGS += -I$(srcdir)/../plugins/PxCone
fastjet_timing_plugins_LDADD += ../plugins/PxCone/libPxConePlugin.la $(FLIBS)
endif
if BUILD_PLUGIN_D0RUNIICONE
fastjet_timing_plugins_CXXFLAGS += -I$(srcdir)/../plugins/D0RunIICone
fastjet_timing_plugins_LDADD += ../plugins/D0RunIICone/libD0RunIIConePlugin.la $(FLIBS)
endif
if BUILD_PLUGIN_TRACKJET
fastjet_timing_plugins_CXXFLAGS += -I$(srcdir)/../plugins/TrackJet
fastjet_timing_plugins_LDADD += ../plugins/TrackJet/libTrackJetPlugin.la
endif
if BUILD_PLUGIN_ATLASCONE
fastjet_timing_plugins_CXXFLAGS += -I$(srcdir)/../plugins/ATLASCone
fastjet_timing_plugins_LDADD += ../plugins/ATLASCone/libATLASConePlugin.la
endif
if BUILD_PLUGIN_EECAMBRIDGE
fastjet_timing_plugins_CXXFLAGS += -I$(srcdir)/../plugins/EECambridge
fastjet_timing_plugins_LDADD += ../plugins/EECambridge/libEECambridgePlugin.la
endif
if BUILD_PLUGIN_JADE
fastjet_timing_plugins_CXXFLAGS += -I$(srcdir)/../plugins/Jade
fastjet_timing_plugins_LDADD += ../plugins/Jade/libJadePlugin.la
endif
if BUILD_PLUGIN_CMSITERATIVECONE
fastjet_timing_plugins_CXXFLAGS += -I$(srcdir)/../plugins/CMSIterativeCone
fastjet_timing_plugins_LDADD += ../plugins/CMSIterativeCone/libCMSIterativeConePlugin.la
endif
if BUILD_PLUGIN_D0RUNICONE
fastjet_timing_plugins_CXXFLAGS += -I${srcdir}/../plugins/D0RunICone
fastjet_timing_plugins_LDADD += ../plugins/D0RunICone/libD0RunIConePlugin.la
endif
# add entries for other plugins here

#TESTS_ENVIRONMENT = $(SHELL)
#TESTS = test-example.sh test-areas.sh
