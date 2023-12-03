enable_testing()

add_executable(
  dummy_test
  dummy_test.cpp
)

add_executable(
  IRModule
  IRModule.cpp
  ../lib/module.cpp
  ../lib/operation.cpp
)

add_executable(
  DFS
  DFS.cpp
  ../lib/module.cpp
  ../lib/operation.cpp
)

target_link_libraries(
  IRModule
  GTest::gtest_main
)

target_link_libraries(
  DFS
  GTest::gtest_main
)

target_include_directories(IRModule PRIVATE ${CMAKE_SOURCE_DIR}/include/)
target_include_directories(DFS PRIVATE ${CMAKE_SOURCE_DIR}/include/)

target_link_libraries(
  dummy_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(dummy_test)