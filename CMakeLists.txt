cmake_minimum_required(VERSION 3.7.0)
include(CMakePrintHelpers)

# set the project name
project(FastJet VERSION 3.4.4)
# set a pre-release version if relevant, e.g. "-beta1"
#set(PROJECT_VERSION_PRERELEASE "")
set(PROJECT_VERSION_PRERELEASE "-devel")

set(FASTJET_VERSION "${PROJECT_VERSION}${PROJECT_VERSION_PRERELEASE}")
# print out the project name and version
cmake_print_variables(PROJECT_NAME PANSCALES_VERSION)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR})

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

if (CMAKE_CXX_COMPILER_ID STREQUAL "IntelLLVM") # Intel compilers are used with -O3
  set(CMAKE_CXX_FLAGS "-O3 -g -Wall -Wshadow")
else()
  set(CMAKE_CXX_FLAGS "-O2 -g -Wall -Wshadow")
endif()
  message("-- Compiling with the following C++ flags: ${CMAKE_CXX_FLAGS}")

# specify that we're using fortran
enable_language(Fortran)
set(CMAKE_Fortran_FLAGS "-O3")

set (panscales_INCLUDE)
set (panscales_LIBRARIES)

#-------------------------------------------------------------------------
# a series of user-defined options
# If you add something here, remember to add a corresponding 
# entry in config.hh.in
#
#option(PSDDREAL "build the code using dd_real for floating-point operations" OFF)

# # momentum class
# option(PANSCALES_MOMENTUM4 "Use Momentum4 as the default 4-vector class instead of the default MomentumM2" OFF)
# if (PANSCALES_MOMENTUM4)
#   set(PANSCALES_MOMENTUM 4)
# else()
#   set(PANSCALES_MOMENTUM 2)
# endif()


#-------------------------------------------------------------------------
# check compiler supports
try_compile(CXX_ABI_COMPILE_SUCCEEDED ${CMAKE_BINARY_DIR}/compiler-tests ${CMAKE_SOURCE_DIR}/compiler-tests/cxx_abi.cc)

if(CXX_ABI_COMPILE_SUCCEEDED)
  message("-- C++ ABI support test passed; enabling demangling")
  option(PANSCALES_ENABLE_DEMANGLING "Enabling C++ demamgling (requires CXX ABI suport)" ON)
  # make sure this percolates to CmdLine
  add_compile_options(-D__CMDLINE_ABI_DEMANGLE__)
else()
  message("-- C++ ABI support test failed; disabling demangling")
  option(PANSCALES_ENABLE_DEMANGLING "Enabling C++ demamgling (requires CXX ABI suport)" OFF)
endif()

#-------------------------------------------------------------------------
# search for dependencies
#find_package(GSL REQUIRED)
#if (PSMPFR4096)
#  find_package(MPFR 2.3.0)  
#endif()

# if (USE_TCMALLOC)
#   
#   find_library(TCMALLOC_LIB NAMES tcmalloc)
#   if (NOT TCMALLOC_LIB)
#     message("-- tcmalloc Not Found (continuing anyway)")
#     if (APPLE)
#       message("----------------------------------------------------------------\n                     ***   WARNING   ***\nThe tcmalloc library has not been found on your system, this can\ncause serious timing penalties on MacOS. Please install it (e.g.\nvia the brew gperftools package) for better performance.\n----------------------------------------------------------------")
#     endif()
#     set(TCMALLOC_LIB "")
#   else()
#     message("-- Found tcmalloc as ${TCMALLOC_LIB}")
#   endif()
# else()
#   set(TCMALLOC_LIB "")
#   message("-- tcmalloc disabled per user request (not recommended on MacOSX)")
# endif()

# support hoppet
add_subdirectory(src)


# generate the configure file
configure_file(config-cmake.hh.in include/fastjet/config_auto.h)

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
