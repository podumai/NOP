cmake_minimum_required(VERSION 3.15)
project(PARSER VERSION 1.2 LANGUAGES CXX)

find_package(GTest CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

include_directories(include/)
include_directories(~/NOP/exception/)
include_directories(~/NOP/bit/)

enable_testing()

add_compile_options(
  -Werror

  -Wall
  -Wextra
  -Wpedantic

  -Wcast-align
  -Wredundant-decls
  -Wsign-promo

  -std=c++23
  -Ofast
)

if (NOT CMAKE_CXX_EXTENSIONS)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif ()

set(parser_exe src/main.cpp)
set(exception_exe ~/NOP/exception/exception.cpp)
set(command_exe src/command.cpp)
set(test_parser_exe src/test.cpp)

add_executable(csvParser ${parser_exe})
add_executable(testParser ${test_parser_exe})
add_library(exception_lib STATIC ${exception_exe})
add_library(command_lib STATIC ${command_exe})
target_link_libraries(csvParser PRIVATE exception_lib command_lib fmt::fmt)
target_link_libraries(testParser PRIVATE GTest::gtest_main exception_lib fmt::fmt)

include(GoogleTest)
gtest_discover_tests(testParser)
