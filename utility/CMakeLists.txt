cmake_minimum_required(VERSION 3.5)
project(UTILITY VERSION 1.0 LANGUAGES CXX)
include_directories("include/")
include_directories("~/NPL/bit/")

#find_package(GTest REQUIRED)

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
  -O0
)

if (NOT CMAKE_CXX_EXTENSIONS)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif ()

set(TEST_EXE src/main.cpp)

add_executable(TEST.OUT ${TEST_EXE})
#target_link_libraries(TEST.OUT ${GTEST_LIBRARIES})