cmake_minimum_required(VERSION 3.5)
project(PARSER VERSION 1.0 LANGUAGES CXX)

include_directories(include/)
include_directories(~/NOP/exception/)

find_package(GTest REQUIRED)

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

add_executable(csv_parser ${parser_exe})
add_executable(test_parser ${test_parser_exe})
add_library(exception_lib STATIC ${exception_exe})
add_library(command_lib STATIC ${command_exe})
target_link_libraries(csv_parser exception_lib command_lib)
target_link_libraries(test_parser exception_lib ${GTEST_LIBRARIES})
