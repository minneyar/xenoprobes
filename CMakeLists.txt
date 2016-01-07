cmake_minimum_required (VERSION 2.8.11)
project (xenoprobes)

add_definitions(-std=c++11)

find_package (Boost COMPONENTS program_options system REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

set (SOURCE_FILES
  src/main.cpp
  src/ore.cpp
  src/ore.h
  src/probe_arrangement.cpp
  src/probe_arrangement.h
  src/probe.cpp
  src/probe.h
  src/probe_optimizer.cpp
  src/probe_optimizer.h
  src/site.cpp
  src/site.h
  src/site_list.cpp
  src/site_list.h
  src/solution.cpp
  src/solution.h
)

add_executable (xenoprobes ${SOURCE_FILES})

target_link_libraries (xenoprobes ${Boost_LIBRARIES})

