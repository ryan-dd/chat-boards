project(ChatBoardServer)
cmake_minimum_required(VERSION 3.0)

find_package(nng CONFIG)

add_executable( ${PROJECT_NAME}
  main.cpp
  )

target_include_directories(${PROJECT_NAME} PUBLIC 
  ${CMAKE_SOURCE_DIR}
  ${nngpp_SOURCE_DIR}/include/
)

target_link_libraries(${PROJECT_NAME} spdlog::spdlog nng::nng nngpp)
