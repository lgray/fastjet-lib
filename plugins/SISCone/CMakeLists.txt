# recurse into the external code
add_subdirectory(siscone)

# sources within FastJet
list(APPEND SISConePlugin_SOURCES
  SISConeBasePlugin.cc
  SISConePlugin.cc
  SISConeSphericalPlugin.cc
)

# build a local library
add_library(SISConePlugin OBJECT ${SISConePlugin_SOURCES})
set_property(TARGET SISConePlugin PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(SISConePlugin PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(SISConePlugin PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/siscone)
target_link_libraries(SISConePlugin PUBLIC fastjet siscone siscone_spherical)

# directly install the header from here
install(FILES fastjet/SISConeBasePlugin.hh fastjet/SISConePlugin.hh fastjet/SISConeSphericalPlugin.hh DESTINATION include/fastjet)

