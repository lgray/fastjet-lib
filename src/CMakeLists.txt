cmake_minimum_required(VERSION 3.7.0)

# specify the C++ standard
#set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_STANDARD_REQUIRED True)
#set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR})
# 
# if (CMAKE_CXX_COMPILER_ID STREQUAL "IntelLLVM") # Intel compilers are used with -O3
#   set(CMAKE_CXX_FLAGS "-O3 -g -Wall -Wshadow")
# else()
#   set(CMAKE_CXX_FLAGS "-O2 -g -Wall -Wshadow")
# endif()
#   message("-- Compiling with the following C++ flags: ${CMAKE_CXX_FLAGS}")



# generate the configure file
#configure_file(config-cmake.hh.in include/fastjet/config-auto.hh)

list(APPEND fastjet_SOURCES
  DnnPlane.cc
  Dnn4piCylinder.cc
  Dnn3piCylinder.cc
  Dnn2piCylinder.cc
  ClusterSequence.cc
  PseudoJet.cc
  Selector.cc
  ClusterSequence_N2.cc
  ClusterSequence_TiledN2.cc
  ClusterSequence_Delaunay.cc
  ClusterSequence_DumbN3.cc
  ClusterSequence_CP2DChan.cc
  ClosestPair2D.cc
  MinHeap.cc
  ClusterSequenceAreaBase.cc
  ClusterSequenceActiveAreaExplicitGhosts.cc
  ClusterSequenceArea.cc
  GhostedAreaSpec.cc
  ClusterSequenceActiveArea.cc
  Voronoi.cc
  ClusterSequenceVoronoiArea.cc
  ClusterSequencePassiveArea.cc
  ClusterSequence1GhostPassiveArea.cc
  PseudoJetStructureBase.cc
  ClusterSequenceStructure.cc
  BasicRandom.cc
  JetDefinition.cc
  Error.cc
  AreaDefinition.cc
  RangeDefinition.cc
  CompositeJetStructure.cc
  FunctionOfPseudoJet.cc
  LimitedWarning.cc
  TilingExtent.cc
  LazyTiling9.cc
  LazyTiling9Alt.cc
  LazyTiling25.cc
  LazyTiling9SeparateGhosts.cc
  RectangularGrid.cc
)

list(APPEND fastjet_INCLUDES
  ../include
)


#-------------------------------------------------------------------------
# For now, disable CGAL
add_compile_options(-DDROP_CGAL)

#-------------------------------------------------------------------------
# update include directories
include_directories(${PROJECT_BINARY_DIR} ${fastjet_INCLUDES})
                     
#-------------------------------------------------------------------------
# main code 
add_library(fastjet SHARED ${fastjet_SOURCES})
target_link_libraries(fastjet PUBLIC ${fastjet_LIBRARIES})
# 
# #-------------------------------------------------------------------------
# # add the 'example-framework' executable
# add_executable(example-framework example-framework.cc)
# target_link_libraries(example-framework panscales ${TCMALLOC_LIB})
