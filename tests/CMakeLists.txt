cmake_minimum_required(VERSION 3.14)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

file(GLOB project_sources ../source/*.cpp)
file(GLOB test_sources *.cpp)

add_executable( ${PROJECT_NAME}-tests ${test_sources} ${project_sources})

target_include_directories(${PROJECT_NAME}-tests PUBLIC ../include)

target_link_libraries(
  ${PROJECT_NAME}-tests
  GTest::gtest
  GTest::gtest_main
)

if(MSVC)
  target_compile_options(${PROJECT_NAME}-tests PRIVATE /W4 /WX)
else()
  target_compile_options(${PROJECT_NAME}-tests PRIVATE -Wall -Wextra -Werror)
endif()

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}-tests)
