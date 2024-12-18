# sources
list(APPEND GridJetPlugin_SOURCES
  GridJetPlugin.cc
)

# update the list of include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../../include ${PROJECT_BINARY_DIR}/include)

# build a local library
add_library(GridJetPlugin OBJECT ${GridJetPlugin_SOURCES})
set_property(TARGET GridJetPlugin PROPERTY POSITION_INDEPENDENT_CODE ON)

# directly install the header from here
install(FILES fastjet/GridJetPlugin.hh DESTINATION include/fastjet)
