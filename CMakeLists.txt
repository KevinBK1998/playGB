cmake_minimum_required(VERSION 3.14)
project(playGB)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(MemoryTest test/MemoryTest.cpp src/Memory.cpp src/Logger.cpp)
add_executable(ProcessorTest test/ProcessorTest.cpp src/Processor.cpp src/Memory.cpp src/Logger.cpp)

target_link_libraries(MemoryTest GTest::gtest_main)
target_link_libraries(ProcessorTest GTest::gtest_main GTest::gmock)

enable_testing()
include(GoogleTest)

gtest_discover_tests(MemoryTest)
gtest_discover_tests(ProcessorTest)
