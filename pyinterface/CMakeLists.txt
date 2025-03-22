cmake_policy(VERSION 3.14)

include(UseSWIG)
find_package(Python COMPONENTS Development)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../pyinterface)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../tools)
include_directories(${PROJECT_BINARY_DIR}/include)
include_directories(${Python_INCLUDE_DIRS})


set_property(SOURCE fastjet.i PROPERTY CPLUSPLUS ON)
set_property(SOURCE fastjet.i PROPERTY USE_LIBRARY_INCLUDE_DIRECTORIES TRUE)
swig_add_library(fastjet_pyext LANGUAGE python SOURCES fastjet.i)
set_property(TARGET fastjet_pyext PROPERTY SWIG_USE_LIBRARY_INCLUDE_DIRECTORIES TRUE)

target_link_libraries(fastjet_pyext PUBLIC fastjet)
target_link_libraries(fastjet_pyext PUBLIC fastjettools)
target_link_libraries(fastjet_pyext PUBLIC ${Python_LIBRARIES})

install(TARGETS fastjet_pyext DESTINATION lib)