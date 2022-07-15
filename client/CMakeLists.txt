project(ChatBoardClient)
cmake_minimum_required(VERSION 3.0)

add_executable( ${PROJECT_NAME}
  main.cpp
  GUI.cpp
  ${CMAKE_BINARY_DIR}/bindings/imgui_impl_glfw.cpp
  ${CMAKE_BINARY_DIR}/bindings/imgui_impl_opengl3.cpp
  ${CMAKE_BINARY_DIR}/bindings/imgui_impl_glfw.h
  ${CMAKE_BINARY_DIR}/bindings/imgui_impl_opengl3.h
  )

target_include_directories(${PROJECT_NAME} PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}
  ${nngpp_SOURCE_DIR}/include/
)

target_compile_definitions(${PROJECT_NAME} PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW)
target_link_libraries(${PROJECT_NAME} PRIVATE
  CONAN_PKG::imgui
  CONAN_PKG::glew
  CONAN_PKG::glfw
  CONAN_PKG::nng
  Shared
  )

