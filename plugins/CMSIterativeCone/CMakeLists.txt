# sources
list(APPEND CMSIterativeConePlugin_SOURCES
  CMSIterativeConePlugin.cc
)

# update the list of include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../../include ${PROJECT_BINARY_DIR}/include)

# build a local library
add_library(CMSIterativeConePlugin OBJECT ${CMSIterativeConePlugin_SOURCES})
set_property(TARGET CMSIterativeConePlugin PROPERTY POSITION_INDEPENDENT_CODE ON)

# directly install the header from here
install(FILES fastjet/CMSIterativeConePlugin.hh DESTINATION include/fastjet)
