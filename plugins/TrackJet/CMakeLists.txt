# sources
list(APPEND TrackJetPlugin_SOURCES
  TrackJetPlugin.cc
)

# update the list of include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../../include ${PROJECT_BINARY_DIR}/include)

# build a local library
add_library(TrackJetPlugin OBJECT ${TrackJetPlugin_SOURCES})
set_property(TARGET TrackJetPlugin PROPERTY POSITION_INDEPENDENT_CODE ON)

# directly install the header from here
install(FILES fastjet/TrackJetPlugin.hh DESTINATION include/fastjet)
