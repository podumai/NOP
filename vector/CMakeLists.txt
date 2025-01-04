cmake_minimum_required(VERSION 3.5)
project(BVECTOR VERSION 1.6 LANGUAGES CXX)

include_directories(include/)
include_directories(../exception/)
include_directories(../bit/)

find_package(GTest CONFIG REQUIRED)

enable_testing()

add_compile_options(
  -Werror

  -Wall
  -Wextra
  -Wpedantic

  -Wcast-align
  -Wold-style-cast
  -Wredundant-decls
  -Wsign-conversion
  -Wsign-promo

  -std=c++23
  -Ofast
)

if (NOT CMAKE_CXX_EXTENSIONS)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif ()

add_executable(TEST src/test.cpp)
add_library(NOP_EXCEPTIONS STATIC ../exception/exception.cpp)
target_link_libraries(TEST PRIVATE GTest::gtest_main NOP_EXCEPTIONS)
include(GoogleTest)
gtest_discover_tests(TEST)
