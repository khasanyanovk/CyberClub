cmake_minimum_required(VERSION 3.14)

project(cyber-club)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB srcs source/*.cpp)
file(GLOB hdrs include/*.hpp)

message(STATUS "[${PROJECT_NAME}] -- Collected sources: ${srcs}")

add_executable(${PROJECT_NAME} main.cpp ${srcs})

target_include_directories(${PROJECT_NAME} PUBLIC ./include)

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror)
endif()

option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
  add_subdirectory(tests)
endif()