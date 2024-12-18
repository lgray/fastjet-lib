# sources
list(APPEND D0RunIIConePlugin_SOURCES
  D0RunIIConePlugin.cc
)

# update the list of include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../../include ${PROJECT_BINARY_DIR}/include)

# build a local library
add_library(D0RunIIConePlugin OBJECT ${D0RunIIConePlugin_SOURCES})
set_property(TARGET D0RunIIConePlugin PROPERTY POSITION_INDEPENDENT_CODE ON)

# directly install the header from here
install(FILES fastjet/D0RunIIConePlugin.hh DESTINATION include/fastjet)
