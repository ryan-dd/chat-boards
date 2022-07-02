cmake_minimum_required(VERSION 3.0)
project(Client CXX)

set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

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

# CONFIG option is important so that CMake doesnt search for modules into the default modules directory
find_package(imgui CONFIG)
find_package(glfw3 CONFIG)
find_package(glew CONFIG)
find_package(spdlog CONFIG)
find_package(nng CONFIG)

add_executable( Client
  main.cpp
  GUI.cpp
  ${CMAKE_BINARY_DIR}/bindings/imgui_impl_glfw.cpp
  ${CMAKE_BINARY_DIR}/bindings/imgui_impl_opengl3.cpp
  ${CMAKE_BINARY_DIR}/bindings/imgui_impl_glfw.h
  ${CMAKE_BINARY_DIR}/bindings/imgui_impl_opengl3.h
  )

target_include_directories(Client PUBLIC 
  ${CMAKE_SOURCE_DIR}/src 
  ${CMAKE_BINARY_DIR}
  ${nngpp_SOURCE_DIR}/include/
)

target_compile_definitions(Client PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW)
target_link_libraries(Client spdlog::spdlog imgui::imgui GLEW::glew_s glfw::glfw nng::nng nngpp)
