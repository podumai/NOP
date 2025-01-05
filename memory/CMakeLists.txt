cmake_minimum_required(VERSION 3.15)
project(SMART_PTR VERSION 1.1 LANGUAGES CXX)

include_directories("include/")
include_directories("../bit/")
include_directories("../exception/")

find_package(fmt CONFIG REQUIRED)

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

set(MAIN_EXE src/main.cpp)
set(EXCEPTION_EXE ../exception/exception.cpp)

add_executable(MAIN ${MAIN_EXE})
add_library(EXCEPTION_LIB STATIC ${EXCEPTION_EXE})
target_link_libraries(MAIN PRIVATE EXCEPTION_LIB fmt::fmt)
