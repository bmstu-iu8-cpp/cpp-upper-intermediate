cmake_minimum_required(VERSION 3.4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_COVERAGE "Build coverage" OFF)

set(
  HUNTER_CACHE_SERVERS
  "https://github.com/bmstu-iu8-cpp-sem-3/hunter-cache"
  CACHE STRING "Default cache server"
)

include("tools/gate/cmake/HunterGate.cmake")

HunterGate(
  URL "https://github.com/cpp-pm/hunter/archive/v0.23.218.tar.gz"
  SHA1 "9a3594a61227967fb92159339ba326701b287b19"
)

project(sync_lecture)

find_package(Threads REQUIRED)
hunter_add_package(Boost)
find_package(Boost CONFIG REQUIRED)

add_executable(server
  # TODO: enum your files and delete this comment
  ${CMAKE_CURRENT_SOURCE_DIR}/sources/source.cpp
)

target_include_directories(server
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(server Boost::boost Threads::Threads)

