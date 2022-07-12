project(Shared)
cmake_minimum_required(VERSION 3.0)

add_library( ${PROJECT_NAME} Data.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${nngpp_SOURCE_DIR}/include/
)

target_link_libraries(${PROJECT_NAME} PUBLIC
  CONAN_PKG::spdlog
  CONAN_PKG::nng
  CONAN_PKG::cereal
  )
