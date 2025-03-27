cmake_minimum_required(VERSION 3.14...3.31)

set(UseSWIG_MODULE_VERSION 2)
include(UseSWIG)

# Really should be 3.17+ and Development.Module, otherwise you require unnecessary libs
find_package(Python COMPONENTS Interpreter Development)

set_property(SOURCE fastjet.i PROPERTY CPLUSPLUS ON)

swig_add_library(_fastjet LANGUAGE python SOURCES fastjet.i OUTPUT_DIR lib)
set_property(TARGET _fastjet PROPERTY SWIG_USE_TARGET_INCLUDE_DIRECTORIES TRUE)
# Unix only, Windows is different
set_property(TARGET _fastjet PROPERTY SUFFIX ".${Python_SOABI}${CMAKE_SHARED_MODULE_SUFFIX}")

target_link_libraries(_fastjet PUBLIC fastjet)
target_link_libraries(_fastjet PUBLIC fastjettools)
target_link_libraries(_fastjet PUBLIC Python::Module)

target_include_directories(_fastjet PUBLIC ../pyinterface)

# Really should go to ${Python_SITEARCH} or into the wheel dir of scikit-build-core
install(TARGETS _fastjet DESTINATION "${Python_SITEARCH}")
install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib" DESTINATION "${Python_SITEARCH}")
