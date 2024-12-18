# List of useful CMake arguments:
#  CMAKE_INSTALL_PREFIX=...     path for the installation
#                               (can be replaced be cmake --install . -- prefix <patth>)
#  FASTJET_ENABLE_DEBUG=ON|OFF  switch debugging info on/off
#
# Note on config file:
#

cmake_minimum_required(VERSION 3.7.0)
include(CMakePrintHelpers)

# set the project name
project(FastJet VERSION 3.4.4)
# set a pre-release version if relevant, e.g. "-beta1"
#set(PROJECT_VERSION_PRERELEASE "")
set(PROJECT_VERSION_PRERELEASE "-devel")



#----------------------------------------------------------------------
# version information
set(FASTJET_VERSION "${PROJECT_VERSION}${PROJECT_VERSION_PRERELEASE}")

set(FASTJET_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(FASTJET_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(FASTJET_VERSION_PATCHLEVEL "${PROJECT_VERSION_PATCH}")
set(FASTJET_VERSION_PRERELEASE "${PROJECT_VERSION_PRERELEASE}")
# build a unique version number
if (FASTJET_VERSION_MINOR LESS 10)
  set(FASTJET_VERSION_MINOR_FOR_NUMBER "0${FASTJET_VERSION_MINOR}")
else()
  set(FASTJET_VERSION_MINOR_FOR_NUMBER "${FASTJET_VERSION_MINOR}")
endif()
if (FASTJET_VERSION_PATCHLEVEL LESS 10)
  set(FASTJET_VERSION_PATCHLEVEL_FOR_NUMBER "0${FASTJET_VERSION_PATCHLEVEL}")
else()
  set(FASTJET_VERSION_PATCHLEVEL_FOR_NUMBER "${FASTJET_VERSION_PATCHLEVEL}")
endif()
set(FASTJET_VERSION_NUMBER "${FASTJET_VERSION_MAJOR}${FASTJET_VERSION_MINOR_FOR_NUMBER}${FASTJET_VERSION_PATCHLEVEL_FOR_NUMBER}")

# print out the project name and version
cmake_print_variables(PROJECT_NAME FASTJET_VERSION)

#----------------------------------------------------------------------
# basic C++ checks
#----------------------------------------------------------------------

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR})

if (CMAKE_CXX_COMPILER_ID STREQUAL "IntelLLVM") # Intel compilers are used with -O3
  set(CMAKE_CXX_FLAGS "-O3 -Wall -Wshadow")
else()
  set(CMAKE_CXX_FLAGS "-O2 -Wall -Wshadow")
endif()
  message("-- Compiling with the following C++ flags: ${CMAKE_CXX_FLAGS}")

# specify that we're using fortran
enable_language(Fortran)
set(CMAKE_Fortran_FLAGS "-O3")

set(fastjet_INCLUDES)
set(fastjet_LIBRARIES)

#-------------------------------------------------------------------------
# a series of user-defined options
# If you add something here, remember to add a corresponding 
# entry in config.hh.in
#
option(FASTJET_ENABLE_DEBUG "Turn on debug compiler information [default=ON]" ON)

# plugins on/off
option(FASTJET_ENABLE_PLUGIN_ATLASCONE        "Enable the ATLASCone plugin [default=ON]"        ON)
option(FASTJET_ENABLE_PLUGIN_CDFCONES         "Enable the CDFCones plugin [default=ON]"         ON)
option(FASTJET_ENABLE_PLUGIN_CMSITERATIVECONE "Enable the CMSIterativeCone plugin [default=ON]" ON)
option(FASTJET_ENABLE_PLUGIN_D0RUNICONE       "Enable the D0RunICone plugin [default=ON]"       ON)
option(FASTJET_ENABLE_PLUGIN_D0RUNIICONE      "Enable the D0RunIICone plugin [default=ON]"      ON)
option(FASTJET_ENABLE_PLUGIN_EECAMBRIDGE      "Enable the EECambridge plugin [default=ON]"      ON)
option(FASTJET_ENABLE_PLUGIN_GRIDJET          "Enable the GridJet plugin [default=ON]"          ON)
option(FASTJET_ENABLE_PLUGIN_JADE             "Enable the Jade plugin [default=ON]"             ON)
option(FASTJET_ENABLE_PLUGIN_NESTEDDEFS       "Enable the NestedDefs plugin [default=ON]"       ON)
option(FASTJET_ENABLE_PLUGIN_PXCONE           "Enable the PxCone plugin [default=ON]"          OFF)
option(FASTJET_ENABLE_PLUGIN_SISCONE          "Enable the SISCone plugin [default=ON]"          ON)
option(FASTJET_ENABLE_PLUGIN_TRACKJET         "Enable the TrackJet plugin [default=ON]"         ON)

#-------------------------------------------------------------------------
# check compiler supports
try_compile(CXX_ABI_COMPILE_SUCCEEDED ${CMAKE_BINARY_DIR}/compiler-tests ${CMAKE_SOURCE_DIR}/compiler-tests/cxx_abi.cc)

if(CXX_ABI_COMPILE_SUCCEEDED)
  message("-- C++ ABI support test passed; enabling demangling")
  option(FASTJET_HAVE_DEMANGLING_SUPPORT "Enabling C++ demamgling (requires CXX ABI suport)" ON)
else()
  message("-- C++ ABI support test failed; disabling demangling")
  option(FASTJET_HAVE_DEMANGLING_SUPPORT "Enabling C++ demamgling (requires CXX ABI suport)" OFF)
endif()

#-------------------------------------------------------------------------
# libm
find_library(LIBM_LIBRARIES m)
list(APPEND fastjet_LIBRARIES ${LIBM_LIBRARIES})

#-------------------------------------------------------------------------
# get into directories:
add_subdirectory(include)  # for basic FJ headers
add_subdirectory(src)      # for the main FJ library
add_subdirectory(tools)    # fastjet toolkit
add_subdirectory(plugins)  # fastjet clustering plugins
add_subdirectory(example)  # for examples (gets built after all the rest)

# generate the configure file
configure_file(config-cmake.hh.in include/fastjet/config_auto.h)
#configure_file(fastjet-config-cmake.in fastjet-config)
#install(FILES fastjet-config DESTINATION bin)

#-------------------------------------------------------------------------
# below some stuff from the PanScales CMake



# #-------------------------------------------------------------------------
# # update include directories
# include_directories(PUBLIC ${CMAKE_SOURCE_DIR} ${PROJECT_BINARY_DIR} ${panscales_INCLUDES})
#                     
# #-------------------------------------------------------------------------
# # main code 
# add_library(panscales SHARED ${panscales_SOURCES})
# target_link_libraries(panscales PUBLIC ${panscales_LIBRARIES})
# 
# #-------------------------------------------------------------------------
# # add the 'example-framework' executable
# add_executable(example-framework example-framework.cc)
# target_link_libraries(example-framework panscales ${TCMALLOC_LIB})
