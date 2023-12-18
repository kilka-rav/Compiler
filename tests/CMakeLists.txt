enable_testing()

add_library(module ../lib/module.cpp
                   ../include/module.h)       

add_executable(
  dummy_test
  dummy_test.cpp
)

add_executable(
  loopAnalyzer
  LoopAnalyzer.cpp
  ../lib/module.cpp
)

add_executable(
  IRModule
  IRModule.cpp
  ../lib/module.cpp/
)

add_executable(
  dominators
  dominators.cpp
  ../lib/module.cpp
)

add_executable(
  DFS
  DFS.cpp
  ../lib/module.cpp
)

target_link_libraries(
  IRModule
  module
  GTest::gtest_main
)

target_link_libraries(
  loopAnalyzer
  GTest::gtest_main
)

target_link_libraries(
  dominators
  GTest::gtest_main
)

target_link_libraries(
  DFS
  GTest::gtest_main
)

target_include_directories(IRModule PRIVATE ${CMAKE_SOURCE_DIR}/include/)
target_include_directories(DFS PRIVATE ${CMAKE_SOURCE_DIR}/include/)
target_include_directories(dominators PRIVATE ${CMAKE_SOURCE_DIR}/include/)
target_include_directories(loopAnalyzer PRIVATE ${CMAKE_SOURCE_DIR}/include/)

target_link_libraries(
  dummy_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(dummy_test)
gtest_discover_tests(dominators)