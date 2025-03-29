# recurse into the external code
add_subdirectory(siscone)

# sources within FastJet
list(APPEND SISConePlugin_SOURCES
  SISConeBasePlugin.cc
  SISConePlugin.cc
  SISConeSphericalPlugin.cc
)

# update the list of include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/siscone ${CMAKE_CURRENT_SOURCE_DIR}/../../include ${PROJECT_BINARY_DIR}/include ${PROJECT_BINARY_DIR}/plugins/SISCone/siscone)

# build a local library
add_library(SISConePlugin OBJECT ${SISConePlugin_SOURCES})
set_property(TARGET SISConePlugin PROPERTY POSITION_INDEPENDENT_CODE ON)
target_link_libraries(SISConePlugin fastjet siscone siscone_spherical)

# directly install the header from here
install(FILES fastjet/SISConeBasePlugin.hh fastjet/SISConePlugin.hh fastjet/SISConeSphericalPlugin.hh DESTINATION include/fastjet)

