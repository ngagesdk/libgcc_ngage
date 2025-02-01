cmake_minimum_required(VERSION 3.10)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if(DEFINED ENV{NGAGESDK})
  set(NGAGESDK $ENV{NGAGESDK})
  set(CMAKE_TOOLCHAIN_FILE ${NGAGESDK}/cmake/ngage-toolchain.cmake)
else()
  message(FATAL_ERROR "The environment variable NGAGESDK needs to be defined.")
endif()

project(gcc_ngage C)

set(project_sources
  ${CMAKE_CURRENT_SOURCE_DIR}/src/libgcc_ngage.c)

set(gcc_ngage_libs
  ${EPOC_PLATFORM}/gcc/lib/gcc-lib/arm-epoc-pe/2.9-psion-98r2/libgcc.a)

add_library(gcc_ngage STATIC ${project_sources})
