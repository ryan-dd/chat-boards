cmake_minimum_required (VERSION 3.7.2)
project(ChatBoards)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
include(cmake/StaticAnalyzers.cmake)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS NO_OUTPUT_DIRS)

set(PROJECT_WARNINGS -Wall -Wshadow -Wnon-virtual-dtor -pedantic)

# Doesn't have a conan pkg, but nngpp is header only so its okay.
if(NOT nngpp_Populated)
  include(FetchContent)
  FetchContent_Declare(
      nngpp
      GIT_REPOSITORY https://github.com/cwzx/nngpp.git
      GIT_TAG        master
  )
  FetchContent_MakeAvailable(nngpp)
endif()

add_subdirectory(shared)
add_subdirectory(client)
add_subdirectory(server)
