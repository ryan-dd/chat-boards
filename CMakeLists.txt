cmake_minimum_required (VERSION 3.7.2)
project(ChatBoards)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
include(cmake/StaticAnalyzers.cmake)

set(PROJECT_WARNINGS -Wall -Wshadow -Wnon-virtual-dtor -pedantic)

# Doesn't have a conan pkg, but the lib is header only so its okay.
if(NOT nngpp_Populated)
  include(FetchContent)
  FetchContent_Declare(
      nngpp
      GIT_REPOSITORY https://github.com/cwzx/nngpp.git
      GIT_TAG        master
  )
  FetchContent_MakeAvailable(nngpp)
endif()

set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

# CONFIG option is important so that CMake doesnt search for modules into the default modules directory
find_package(spdlog CONFIG)
find_package(nng CONFIG)

add_subdirectory(client)
add_subdirectory(server)
# add_subdirectory(test)
