# sources
list(APPEND D0RunIConePlugin_SOURCES
  D0RunIBaseConePlugin.cc
)

# update the list of include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../../include ${PROJECT_BINARY_DIR}/include)

# build a local library
add_library(D0RunIConePlugin OBJECT ${D0RunIConePlugin_SOURCES})
set_property(TARGET D0RunIConePlugin PROPERTY POSITION_INDEPENDENT_CODE ON)

# directly install the header from here
install(FILES fastjet/D0RunIConePlugin.hh DESTINATION include/fastjet)
