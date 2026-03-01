cmake_minimum_required(VERSION 3.10)

project(cyber-club)
set(CMAKE_CXX_STANDARD 20)

file(GLOB src source/*.cpp)

message(STATUS "[${PROJECT_NAME}] -- Collected sources: ${src}")

add_executable(${PROJECT_NAME} main.cpp ${src})

target_include_directories(${PROJECT_NAME} PUBLIC ./iniclude)