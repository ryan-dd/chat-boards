cmake_minimum_required (VERSION 3.7.2)
project(SampleProject)

set(CMAKE_CXX_STANDARD 20)

# include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
# conan_basic_setup(TARGETS NO_OUTPUT_DIRS)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
include(cmake/StaticAnalyzers.cmake)

set(PROJECT_WARNINGS -Wall -Wshadow -Wnon-virtual-dtor -pedantic)

add_subdirectory(src)
# add_subdirectory(test)
