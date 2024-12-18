# sources
list(APPEND ATLASConePlugin_SOURCES
  Jet.cc
  JetConeFinderTool.cc
  JetSplitMergeTool.cc
  ATLASConePlugin.cc
)

# headers that will eventually have to be installed?
# CommonUtils.hh JetConeFinderTool.hh JetDistances.hh Jet.hh JetSplitMergeTool.hh LorentzVector.hh

# update the list of include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../../include ${PROJECT_BINARY_DIR}/include)

# build a local library
add_library(ATLASConePlugin OBJECT ${ATLASConePlugin_SOURCES})
set_property(TARGET ATLASConePlugin PROPERTY POSITION_INDEPENDENT_CODE ON)

# directly install the header from here
install(FILES fastjet/ATLASConePlugin.hh DESTINATION include/fastjet)
