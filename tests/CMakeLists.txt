enable_testing()

add_library(module ../lib/module.cpp
                   ../include/module.h)       

add_executable(
  dummy_test
  dummy_test.cpp
)

add_executable(optimization_test
  optimization.cpp
)

add_executable(inline
  inline.cpp
)

add_executable(check_elimination
check_elimination.cpp
)

add_executable(
  loopAnalyzer
  LoopAnalyzer.cpp
  ../lib/module.cpp
)

add_executable(
  liveness
  liveness.cpp
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
  regAlloc
  regAlloc.cpp
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
  liveness
  GTest::gtest_main
)

target_link_libraries(
  dominators
  GTest::gtest_main
)

target_link_libraries(
  DFS
  GTest::gtest_main
  module
)

target_link_libraries(
  regAlloc
  GTest::gtest_main
  module
)

target_link_libraries(optimization_test
  GTest::gtest_main
  module
  optimization)

  target_link_libraries(check_elimination
  GTest::gtest_main
  module
  optimization)

target_link_libraries(inline
  GTest::gtest_main
  module
  optimization)

target_include_directories(IRModule PRIVATE ${CMAKE_SOURCE_DIR}/include/)
target_include_directories(DFS PRIVATE ${CMAKE_SOURCE_DIR}/include/)
target_include_directories(dominators PRIVATE ${CMAKE_SOURCE_DIR}/include/)
target_include_directories(loopAnalyzer PRIVATE ${CMAKE_SOURCE_DIR}/include/)
target_include_directories(liveness PRIVATE ${CMAKE_SOURCE_DIR}/include/)
target_include_directories(regAlloc PRIVATE ${CMAKE_SOURCE_DIR}/include/)
target_include_directories(optimization_test PRIVATE ${CMAKE_SOURCE_DIR}/include/)
target_include_directories(check_elimination PRIVATE ${CMAKE_SOURCE_DIR}/include())
target_include_directories(inline PRIVATE ${CMAKE_SOURCE_DIR}/include())

target_link_libraries(
  dummy_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(dummy_test)
gtest_discover_tests(dominators)
