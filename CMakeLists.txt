cmake_minimum_required(VERSION 3.18)

set(projectname MPItasks)

include_directories(${MPI_INCLUDE_PATH})

project(${projectname} MPI_CXX_COMPILER)

#set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} ${MPI_COMPILE_FLAGS})
#set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS} ${MPI_LINK_FLAGS})


add_subdirectory(HelloWorld)
#
#set(projectname libgstreamer_plugins_milandr)
#
#project(${projectname} C CXX)
#
#if(NOT CMAKE_C_STANDARD)
#    set(CMAKE_C_STANDARD 99)
#endif()
#
#if(NOT CMAKE_CXX_STANDARD)
#    set(CMAKE_CXX_STANDARD 17)
#endif()
#
#list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
#
#include(MacroLogFeature)
#include(MacroAddObject)
#
#find_package(MPI)
#
#add_objects_begin()
#add_object("helloworld" HelloWorld TRUE GST_APP_FOUND "Base task1")
#add_objects_end()