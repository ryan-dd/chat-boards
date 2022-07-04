project(ChatBoardServer)
cmake_minimum_required(VERSION 3.0)

add_executable( ${PROJECT_NAME}
  main.cpp
  )

target_include_directories(${PROJECT_NAME} PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME} Shared)
