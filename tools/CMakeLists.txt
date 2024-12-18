# recurse into the header directory
add_subdirectory(fastjet)

# local sources
list(APPEND fastjettools_SOURCES
  BackgroundEstimatorBase.cc 
  JetMedianBackgroundEstimator.cc 
  GridMedianBackgroundEstimator.cc 
  Recluster.cc 
  Filter.cc
  Pruner.cc
  Subtractor.cc
  MassDropTagger.cc
  RestFrameNSubjettinessTagger.cc
  CASubJetTagger.cc 
  JHTopTagger.cc
  TopTaggerBase.cc
)

list(APPEND fastjet_INCLUDES
  ${CMAKE_CURRENT_SOURCE_DIR}/../include ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_BINARY_DIR}/include
)

#-------------------------------------------------------------------------
# update include directories
include_directories(${fastjet_INCLUDES})
                     
#-------------------------------------------------------------------------
# main code 
add_library(fastjettools SHARED ${fastjettools_SOURCES})
target_link_libraries(fastjettools PUBLIC ${fastjet_LIBRARIES} fastjet)
install(TARGETS fastjettools DESTINATION lib)
