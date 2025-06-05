cmake_minimum_required(VERSION 3.5)
project(VECTOR VERSION 2.0 LANGUAGES CXX)

find_package(GTest CONFIG REQUIRED)

enable_testing()

if (NOT CMAKE_CXX_EXTENSIONS)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif ()

add_executable(TEST test/test.cpp)
target_include_directories(TEST PRIVATE .)
target_compile_options(TEST PRIVATE -Werror -Wall -Wextra -Wpedantic -Wcast-align -Wold-style-cast -Wredundant-decls -Wsign-conversion -Wsign-promo -std=c++23 -O3)
target_link_libraries(TEST PRIVATE GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(TEST)
