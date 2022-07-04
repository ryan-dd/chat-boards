project(Shared)
cmake_minimum_required(VERSION 3.0)

add_library( ${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE 
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${nngpp_SOURCE_DIR}/include/
)

target_link_libraries(${PROJECT_NAME} INTERFACE
  CONAN_PKG::spdlog
  CONAN_PKG::nng
  CONAN_PKG::cereal
  )
