
# specify that we're using fortran
enable_language(Fortran)
set(CMAKE_Fortran_FLAGS "-O3")

# sources
list(APPEND PxConePlugin_SOURCES
  pxcone.f
  PxConePlugin.cc
)

# update the list of include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../../include ${PROJECT_BINARY_DIR}/include)

# build a local library
add_library(PxConePlugin OBJECT ${PxConePlugin_SOURCES})
set_property(TARGET PxConePlugin PROPERTY POSITION_INDEPENDENT_CODE ON)

# directly install the header from here
install(FILES fastjet/PxConePlugin.hh DESTINATION include/fastjet)
