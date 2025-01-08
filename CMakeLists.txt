cmake_minimum_required(VERSION 3.10)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if(DEFINED ENV{NGAGESDK})
    set(NGAGESDK $ENV{NGAGESDK})
    set(CMAKE_TOOLCHAIN_FILE ${NGAGESDK}/cmake/ngage-toolchain.cmake)
else()
    message(FATAL_ERROR "The environment variable NGAGESDK needs to be defined.")
endif()

project(gcc_ngage C CXX)

set(project_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/src/libgcc_ngage.c)

set(GCC_COMN_DEFS -D__SYMBIAN32__ -D__GCC32__ -D__EPOC32__ -D__MARM__ -D__MARM_ARMI__)
set(GCC_MODE_DEFS -DNDEBUG -D_UNICODE)
set(GCC_DEFS ${GCC_COMN_DEFS} ${GCC_MODE_DEFS})

set(gcc_ngage_libs
    ${EPOC_PLATFORM}/gcc/lib/gcc-lib/arm-epoc-pe/2.9-psion-98r2/libgcc.a)

add_library(gcc_ngage STATIC ${project_sources})

target_compile_definitions(
    gcc_ngage
    PUBLIC
    ${GCC_DEFS})

target_compile_options(
    gcc_ngage
    PUBLIC
    -O2)
