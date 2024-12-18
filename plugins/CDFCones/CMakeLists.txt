# sources
list(APPEND CDFConesPlugin_SOURCES
  CDFcode/CalTower.cc
  CDFcode/Centroid.cc
  CDFcode/Cluster.cc
  CDFcode/ClusterComparisons.cc
  CDFcode/JetCluAlgorithm.cc
  CDFcode/LorentzVector.cc
  CDFcode/MidPointAlgorithm.cc
  CDFcode/PhysicsTower.cc
  CDFJetCluPlugin.cc
  CDFMidPointPlugin.cc
)

# update the list of include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/CDFcode ${CMAKE_CURRENT_SOURCE_DIR}/../../include ${PROJECT_BINARY_DIR}/include)

# build a local library
add_library(CDFConesPlugin OBJECT ${CDFConesPlugin_SOURCES})
set_property(TARGET CDFConesPlugin PROPERTY POSITION_INDEPENDENT_CODE ON)

# directly install the header from here
install(FILES fastjet/CDFJetCluPlugin.hh fastjet/CDFMidPointPlugin.hh DESTINATION include/fastjet)
