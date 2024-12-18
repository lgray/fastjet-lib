# sources
list(APPEND EECambridgePlugin_SOURCES
  EECambridgePlugin.cc
)

# update the list of include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../../include ${PROJECT_BINARY_DIR}/include)

# build a local library
add_library(EECambridgePlugin OBJECT ${EECambridgePlugin_SOURCES})
set_property(TARGET EECambridgePlugin PROPERTY POSITION_INDEPENDENT_CODE ON)

# directly install the header from here
install(FILES fastjet/EECambridgePlugin.hh DESTINATION include/fastjet)
