enable_testing()

add_executable(
  dummy_test
  dummy_test.cpp
)

add_executable(
  EmptyModule
  EmptyModule.cpp
  ../lib/module.cpp
  ../lib/operation.cpp
)
target_link_libraries(
  EmptyModule
  GTest::gtest_main
)
target_include_directories(EmptyModule PRIVATE ${CMAKE_SOURCE_DIR}/include/)

target_link_libraries(
  dummy_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(dummy_test)