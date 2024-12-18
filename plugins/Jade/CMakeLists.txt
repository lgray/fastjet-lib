# sources
list(APPEND JadePlugin_SOURCES
  JadePlugin.cc
)

# update the list of include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../../include ${PROJECT_BINARY_DIR}/include)

# build a local library
add_library(JadePlugin OBJECT ${JadePlugin_SOURCES})
set_property(TARGET JadePlugin PROPERTY POSITION_INDEPENDENT_CODE ON)

# directly install the header from here
install(FILES fastjet/JadePlugin.hh DESTINATION include/fastjet)
