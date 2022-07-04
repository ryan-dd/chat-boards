project(Shared)
cmake_minimum_required(VERSION 3.0)

find_package(nng CONFIG)
find_package(cereal CONFIG)

add_library( ${PROJECT_NAME}
  ${CMAKE_CURRENT_SOURCE_DIR}/CerealSerializer.cpp
  )

target_include_directories(${PROJECT_NAME} PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${nngpp_SOURCE_DIR}/include/
)

target_link_libraries(${PROJECT_NAME} spdlog::spdlog nng::nng nngpp cereal::cereal)
