# sources
list(APPEND NestedDefsPlugin_SOURCES
  NestedDefsPlugin.cc
)

# update the list of include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../../include ${PROJECT_BINARY_DIR}/include)

# build a local library
add_library(NestedDefsPlugin OBJECT ${NestedDefsPlugin_SOURCES})
set_property(TARGET NestedDefsPlugin PROPERTY POSITION_INDEPENDENT_CODE ON)

# directly install the header from here
install(FILES fastjet/NestedDefsPlugin.hh DESTINATION include/fastjet)
