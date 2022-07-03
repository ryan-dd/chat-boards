project(ChatBoardClient)
cmake_minimum_required(VERSION 3.0)

find_package(imgui CONFIG)
find_package(glfw3 CONFIG)
find_package(glew CONFIG)
find_package(nng CONFIG)
find_package(cereal CONFIG)

add_executable( ${PROJECT_NAME}
  main.cpp
  GUI.cpp
  ${CMAKE_BINARY_DIR}/bindings/imgui_impl_glfw.cpp
  ${CMAKE_BINARY_DIR}/bindings/imgui_impl_opengl3.cpp
  ${CMAKE_BINARY_DIR}/bindings/imgui_impl_glfw.h
  ${CMAKE_BINARY_DIR}/bindings/imgui_impl_opengl3.h
  )

target_include_directories(${PROJECT_NAME} PUBLIC 
  ${CMAKE_SOURCE_DIR}/src 
  ${CMAKE_BINARY_DIR}
  ${nngpp_SOURCE_DIR}/include/
)

target_compile_definitions(${PROJECT_NAME} PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW)
target_link_libraries(${PROJECT_NAME} PUBLIC
  spdlog::spdlog 
  imgui::imgui 
  GLEW::glew_s 
  glfw::glfw 
  nng::nng 
  nngpp
  cereal::cereal
  )
