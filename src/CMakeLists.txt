add_library(lib${PROJECT_NAME}
  SampleClass.cpp
)

target_link_libraries(lib${PROJECT_NAME} PUBLIC
  CONAN_PKG::spdlog
)

target_include_directories(lib${PROJECT_NAME} PUBLIC 
  ${CMAKE_SOURCE_DIR}/src 
)

target_compile_options(lib${PROJECT_NAME} INTERFACE ${PROJECT_WARNINGS})
