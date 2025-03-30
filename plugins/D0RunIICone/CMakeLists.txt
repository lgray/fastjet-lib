# sources
list(APPEND D0RunIIConePlugin_SOURCES
  D0RunIIConePlugin.cc
)

# build a local library
add_library(D0RunIIConePlugin OBJECT ${D0RunIIConePlugin_SOURCES})
set_property(TARGET D0RunIIConePlugin PROPERTY POSITION_INDEPENDENT_CODE ON)

target_link_libraries(D0RunIIConePlugin PUBLIC fastjet)

# directly install the header from here
install(FILES fastjet/D0RunIIConePlugin.hh DESTINATION include/fastjet)
