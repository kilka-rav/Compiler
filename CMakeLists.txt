cmake_minimum_required(VERSION 3.14)
project(Compiler)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_CXX_EXTENSIONS)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

set(IR_EXEC ir)

add_executable(${IR_EXEC} main.cpp
                          lib/module.cpp
                          )
target_include_directories(${IR_EXEC} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/)

add_library(optimization lib/optimization.cpp
                         include/optimization.h
)

target_link_libraries(${IR_EXEC} optimization)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_subdirectory(tests)