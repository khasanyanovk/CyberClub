cmake_minimum_required(VERSION 3.10)

project(cyber-club)
set(CMAKE_CXX_STANDARD 20)

file(GLOB srcs source/*.cpp)
file(GLOB hdrs include/*.hpp)

message(STATUS "[${PROJECT_NAME}] -- Collected sources: ${srcs}")

add_executable(${PROJECT_NAME} main.cpp ${srcs})

target_include_directories(${PROJECT_NAME} PUBLIC ./include)