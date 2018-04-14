
cmake_minimum_required(VERSION 2.6)

# project name
project(tutorial)

# version number
set(tutorial_VERSION_MAJOR 1)
set(tutorial_VERSION_MINOR 0)


# path
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
# where to put executables
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
# where to put libraries
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})


# configure header file
configure_file(
        "${CMAKE_SOURCE_DIR}/src/tut.h.in"
        "${CMAKE_SOURCE_DIR}/src/tut.h"
)

# add to search path for include files
include_directories("${CMAKE_SOURCE_DIR}")


# conditional build and link math
if (USE_MYMATH)
    include_directories("${CMAKE_SOURCE_DIR}/utils")
    add_subdirectory(utils)
    set(EXTRA_LIBS ${EXTRA_LIBS} utils)

    # library
    add_library(utils math.cpp)
endif(USE_MYMATH)




# add executable
add_executable(tutorial src/tut.cpp)


# add new library to executable
target_link_libraries(tutorial ${EXTRA_LIBS})
