cmake_minimum_required (VERSION 3.8)
project (xenoprobes
    VERSION 1.0.0
    LANGUAGES CXX)

find_package (Threads)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
find_package (Boost COMPONENTS filesystem program_options system REQUIRED)

if(MSVC)
  add_compile_options(
    $<$<CONFIG:>:/MT> #---------|
    $<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
    $<$<CONFIG:Release>:/MT> #--|
  )
endif()

add_executable(${PROJECT_NAME})

if (MSVC)
  set_target_properties(${PROJECT_NAME} PROPERTIES
      MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
      CXX_STANDARD 14
      )
else()
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)
endif()

target_sources(${PROJECT_NAME} PRIVATE
  src/main.cpp
  src/ore.cpp
  src/probe_arrangement.cpp
  src/probe.cpp
  src/probe_optimizer.cpp
  src/site.cpp
  src/site_list.cpp
  src/solution.cpp
  src/semaphore.cpp
)
target_link_libraries (${PROJECT_NAME}
  PRIVATE
  Boost::filesystem
  Boost::program_options
  Boost::system
  Threads::Threads
  $<IF:$<BOOL:MSVC>,,-static>
)

