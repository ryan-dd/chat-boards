project(ChatBoardServer)
cmake_minimum_required(VERSION 3.0)

add_executable( ${PROJECT_NAME}
  main.cpp
  )

target_include_directories(${PROJECT_NAME} PRIVATE 
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${nngpp_SOURCE_DIR}/include/
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
  CONAN_PKG::nng
  Shared
  )

