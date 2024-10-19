cmake_minimum_required(VERSION 3.28)
project(leveldb_example)

# C++11 standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# LevelDB configuration
set(LEVELDB_DIR ${CMAKE_SOURCE_DIR}/third-party/leveldb)
# Disable tests for levelDB by setting this option before add_subdirectory
set(LEVELDB_BUILD_TESTS OFF CACHE BOOL "Build LevelDB tests" FORCE)
set(LEVELDB_BUILD_BENCHMARKS OFF CACHE BOOL "Build LevelDB benchmarks" FORCE)

# Add LevelDB
add_subdirectory(${LEVELDB_DIR})
include_directories(${LEVELDB_DIR})
include_directories(${LEVELDB_DIR}/include)

set(SOURCE_FILES
        main.cc
)

add_executable(leveldb_example ${SOURCE_FILES})
target_link_libraries(leveldb_example leveldb)
