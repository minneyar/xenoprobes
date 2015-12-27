cmake_minimum_required (VERSION 2.8.11)
project (xenoprobes)

if (CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=gnu++0x)
endif()

find_package (Boost COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

set (SOURCE_FILES
  src/optimize.cpp)

add_executable (xenoprobes ${SOURCE_FILES})

target_link_libraries (xenoprobes ${Boost_LIBRARIES})

