enable_testing()

add_executable(unit_tests 
  SampleTest.cpp
)

target_link_libraries(unit_tests 
  CONAN_PKG::gtest 
  lib${PROJECT_NAME}
)

