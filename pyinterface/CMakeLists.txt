cmake_minimum_required(VERSION 3.16...3.31)

set(UseSWIG_MODULE_VERSION 2)
include(UseSWIG)

if (CMAKE_VERSION VERSION_LESS 3.18)
  set(DEV_MODULE Development)
else()
  set(DEV_MODULE Development.Module)
endif()

find_package(Python 3.8 COMPONENTS Interpreter ${DEV_MODULE} REQUIRED)

set_property(SOURCE fastjet_swig.i PROPERTY CPLUSPLUS ON)
set_property(SOURCE fastjet_swig.i PROPERTY SWIG_MODULE_NAME __init__)

swig_add_library(fastjet_swig LANGUAGE python SOURCES fastjet_swig.i OUTPUT_DIR fastjet_swig)
set_property(TARGET fastjet_swig PROPERTY SWIG_USE_TARGET_INCLUDE_DIRECTORIES TRUE)
# Unix only, Windows is different
set_property(TARGET fastjet_swig PROPERTY SUFFIX ".${Python_SOABI}${CMAKE_SHARED_MODULE_SUFFIX}")

target_link_libraries(fastjet_swig PUBLIC fastjet)
target_link_libraries(fastjet_swig PUBLIC fastjettools)
target_link_libraries(fastjet_swig PUBLIC Python::Module)

target_include_directories(fastjet_swig PUBLIC ../pyinterface)

# Really should go to ${Python_SITEARCH} or into the wheel dir of scikit-build-core
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/fastjet_swig/__init__.py" DESTINATION "${Python_SITEARCH}/${FASTJET_PYTHON_PACKAGE_NAME}/")
install(TARGETS fastjet_swig DESTINATION "${Python_SITEARCH}/${FASTJET_PYTHON_PACKAGE_NAME}")

