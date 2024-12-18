#-------------------------------------------------------------------------
# update include directories
list(APPEND fastjet_INCLUDES
  ${CMAKE_CURRENT_SOURCE_DIR}/../include ${CMAKE_CURRENT_SOURCE_DIR}/../tools ${PROJECT_BINARY_DIR}/include
)

include_directories(${fastjet_INCLUDES})

# example 01-basic
add_executable(01-basic 01-basic.cc)
target_link_libraries(01-basic fastjet)

# example 02-jetdef
add_executable(02-jetdef 02-jetdef.cc)
target_link_libraries(02-jetdef fastjet)

# example 03-plugin
add_executable(03-plugin 03-plugin.cc)
target_link_libraries(03-plugin fastjet)
if (FASTJET_ENABLE_PLUGIN_SISCONE)
  target_include_directories(03-plugin PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../plugins/SISCone)
  target_link_libraries(03-plugin fastjetplugins siscone siscone_spherical)
endif()

# example 04-constituents
add_executable(04-constituents 04-constituents.cc)
target_link_libraries(04-constituents fastjet)

# example 05-eplus_eminus
add_executable(05-eplus_eminus 05-eplus_eminus.cc)
target_link_libraries(05-eplus_eminus fastjet)

# example 06-area
add_executable(06-area 06-area.cc)
target_link_libraries(06-area fastjet)

# example 07-subtraction
add_executable(07-subtraction 07-subtraction.cc)
target_link_libraries(07-subtraction fastjettools)

# example 08-selector
add_executable(08-selector 08-selector.cc)
target_link_libraries(08-selector fastjet)

# example 09-user_info
add_executable(09-user_info 09-user_info.cc)
target_link_libraries(09-user_info fastjet)

# example 10-subjets
add_executable(10-subjets 10-subjets.cc)
target_link_libraries(10-subjets fastjet)

# example 11-filter
add_executable(11-filter 11-filter.cc)
target_link_libraries(11-filter fastjettools)

# example 12-boosted_higgs
add_executable(12-boosted_higgs 12-boosted_higgs.cc)
target_link_libraries(12-boosted_higgs fastjettools)

# example 13-boosted_top
add_executable(13-boosted_top 13-boosted_top.cc)
target_link_libraries(13-boosted_top fastjettools)

# example 14-groomers
add_executable(14-groomers 14-groomers.cc)
target_link_libraries(14-groomers fastjettools)

# fastjet_timing_plugins
set(fastjetplugin_INCLUDES)
set(fastjetplugin_LIBRARIES fastjetplugins fastjettools)
if (FASTJET_ENABLE_PLUGIN_ATLASCONE)
  list(APPEND fastjetplugin_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/../plugins/ATLASCone)
endif()
if (FASTJET_ENABLE_PLUGIN_CDFCONES)
  list(APPEND fastjetplugin_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/../plugins/CDFCones)
endif()
if (FASTJET_ENABLE_PLUGIN_CMSITERATIVECONE)
  list(APPEND fastjetplugin_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/../plugins/CMSIterativeCone)
endif()
if (FASTJET_ENABLE_PLUGIN_D0RUNICONE)
  list(APPEND fastjetplugin_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/../plugins/D0RunICone)
endif()
if (FASTJET_ENABLE_PLUGIN_D0RUNIICONE)
  list(APPEND fastjetplugin_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/../plugins/D0RunIICone)
endif()
if (FASTJET_ENABLE_PLUGIN_EECAMBRIDGE)
  list(APPEND fastjetplugin_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/../plugins/EECambridge)
endif()
if (FASTJET_ENABLE_PLUGIN_GRIDJET)
  list(APPEND fastjetplugin_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/../plugins/GridJet)
endif()
if (FASTJET_ENABLE_PLUGIN_JADE)
  list(APPEND fastjetplugin_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/../plugins/Jade)
endif()
if (FASTJET_ENABLE_PLUGIN_NESTEDDEFS)
  list(APPEND fastjetplugin_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/../plugins/NestedDefs)
endif()
if (FASTJET_ENABLE_PLUGIN_PXCONE)
  list(APPEND fastjetplugin_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/../plugins/PxCone)
endif()
if (FASTJET_ENABLE_PLUGIN_SISCONE)
  list(APPEND fastjetplugin_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/../plugins/SISCone)
  list(APPEND fastjetplugin_LIBRARIES siscone siscone_spherical)
endif()
if (FASTJET_ENABLE_PLUGIN_TRACKJET)
  list(APPEND fastjetplugin_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/../plugins/TrackJet)
endif()
add_executable(fastjet_timing_plugins fastjet_timing_plugins.cc CmdLine.cc)
target_link_libraries(fastjet_timing_plugins ${fastjetplugin_LIBRARIES})
target_include_directories(fastjet_timing_plugins PUBLIC ${fastjetplugin_INCLUDES})



# check_PROGRAMS = fastjet_example fastjet_areas

# # illustrates the usage of plugins
# if BUILD_PLUGIN_SISCONE
# noinst_PROGRAMS += 03-plugin
# 03_plugin_SOURCES = 03-plugin.cc
# #03_plugin_SHORTNAME = 03
# 03_plugin_CXXFLAGS = $(AM_CXXFLAGS) $(CGAL_CPPFLAGS) -I$(srcdir)/../include
# 03_plugin_LDADD = ../src/libfastjet.la $(CGAL_LIBS)
# 03_plugin_CXXFLAGS += -I$(srcdir)/../plugins/SISCone -I$(srcdir)/../plugins/SISCone/siscone
# 03_plugin_LDADD += ../plugins/SISCone/libSISConePlugin.la ../plugins/SISCone/siscone/siscone/libsiscone.la ../plugins/SISCone/siscone/siscone/spherical/libsiscone_spherical.la
# else
# EXTRA_DIST += 03-plugin.cc
# endif

# 
# # backward-compatible + make-check-related things
# fastjet_example_SOURCES = fastjet_example.cc
# fastjet_example_CXXFLAGS = $(AM_CXXFLAGS) $(CGAL_CPPFLAGS) -I$(srcdir)/../include
# fastjet_example_LDADD = ../src/libfastjet.la $(CGAL_LIBS)
# 
# fastjet_areas_SOURCES = fastjet_areas.cc
# fastjet_areas_CXXFLAGS = $(AM_CXXFLAGS) $(CGAL_CPPFLAGS) -I$(srcdir)/../include
# fastjet_areas_LDADD = ../src/libfastjet.la $(CGAL_LIBS)
# if BUILD_PLUGIN_SISCONE
# fastjet_areas_CXXFLAGS += -I$(srcdir)/../plugins/SISCone -I$(srcdir)/../plugins/SISCone/siscone
# fastjet_areas_LDADD += ../plugins/SISCone/libSISConePlugin.la ../plugins/SISCone/siscone/siscone/libsiscone.la ../plugins/SISCone/siscone/siscone/spherical/libsiscone_spherical.la
# endif
# 
# fastjet_timing_plugins_SOURCES = fastjet_timing_plugins.cc CmdLine.cc
# fastjet_timing_plugins_CXXFLAGS = $(AM_CXXFLAGS) $(CGAL_CPPFLAGS) -I$(srcdir)/../include
# fastjet_timing_plugins_LDADD = ../src/libfastjet.la $(CGAL_LIBS)
# fastjet_timing_plugins_CXXFLAGS += -I$(srcdir)/../tools
# fastjet_timing_plugins_LDADD += ../tools/libfastjettools.la 
# if BUILD_PLUGIN_SISCONE
# fastjet_timing_plugins_CXXFLAGS += -I$(srcdir)/../plugins/SISCone -I$(srcdir)/../plugins/SISCone/siscone
# fastjet_timing_plugins_LDADD += ../plugins/SISCone/libSISConePlugin.la ../plugins/SISCone/siscone/siscone/libsiscone.la ../plugins/SISCone/siscone/siscone/spherical/libsiscone_spherical.la
# endif
# if BUILD_PLUGIN_CDFCONES
# fastjet_timing_plugins_CXXFLAGS += -I$(srcdir)/../plugins/CDFCones
# fastjet_timing_plugins_LDADD += ../plugins/CDFCones/libCDFConesPlugin.la ../plugins/CDFCones/CDFcode/libCDFcode.la
# endif
# if BUILD_PLUGIN_PXCONE
# fastjet_timing_plugins_CXXFLAGS += -I$(srcdir)/../plugins/PxCone
# fastjet_timing_plugins_LDADD += ../plugins/PxCone/libPxConePlugin.la $(FLIBS)
# endif
# if BUILD_PLUGIN_D0RUNIICONE
# fastjet_timing_plugins_CXXFLAGS += -I$(srcdir)/../plugins/D0RunIICone
# fastjet_timing_plugins_LDADD += ../plugins/D0RunIICone/libD0RunIIConePlugin.la $(FLIBS)
# endif
# if BUILD_PLUGIN_TRACKJET
# fastjet_timing_plugins_CXXFLAGS += -I$(srcdir)/../plugins/TrackJet
# fastjet_timing_plugins_LDADD += ../plugins/TrackJet/libTrackJetPlugin.la
# endif
# if BUILD_PLUGIN_ATLASCONE
# fastjet_timing_plugins_CXXFLAGS += -I$(srcdir)/../plugins/ATLASCone
# fastjet_timing_plugins_LDADD += ../plugins/ATLASCone/libATLASConePlugin.la
# endif
# if BUILD_PLUGIN_EECAMBRIDGE
# fastjet_timing_plugins_CXXFLAGS += -I$(srcdir)/../plugins/EECambridge
# fastjet_timing_plugins_LDADD += ../plugins/EECambridge/libEECambridgePlugin.la
# endif
# if BUILD_PLUGIN_JADE
# fastjet_timing_plugins_CXXFLAGS += -I$(srcdir)/../plugins/Jade
# fastjet_timing_plugins_LDADD += ../plugins/Jade/libJadePlugin.la
# endif
# if BUILD_PLUGIN_CMSITERATIVECONE
# fastjet_timing_plugins_CXXFLAGS += -I$(srcdir)/../plugins/CMSIterativeCone
# fastjet_timing_plugins_LDADD += ../plugins/CMSIterativeCone/libCMSIterativeConePlugin.la
# endif
# if BUILD_PLUGIN_D0RUNICONE
# fastjet_timing_plugins_CXXFLAGS += -I${srcdir}/../plugins/D0RunICone
# fastjet_timing_plugins_LDADD += ../plugins/D0RunICone/libD0RunIConePlugin.la
# endif
# if BUILD_PLUGIN_GRIDJET
# fastjet_timing_plugins_CXXFLAGS += -I${srcdir}/../plugins/GridJet
# fastjet_timing_plugins_LDADD += ../plugins/GridJet/libGridJetPlugin.la
# endif
# 
# # add entries for other plugins here
# 
# #TESTS_ENVIRONMENT = $(SHELL)
# #TESTS = test-example.sh test-areas.sh
