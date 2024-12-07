cmake_minimum_required (VERSION 3.28)
project (xenoprobes
    VERSION 1.0.0
    DESCRIPTION "Calculate the optimal Data Probe layout for Xenoblade Chronicles X"
    LANGUAGES CXX
    HOMEPAGE_URL "https://github.com/minneyar/xenoprobes"
)
set(PROJECT_DISPLAY_NAME "XenoProbes")
set(PROJECT_ORGANIZATION_NAME "minneyar")
string(TIMESTAMP PROJECT_BUILD_TIMESTAMP "%s" UTC)
find_package(Git REQUIRED)
execute_process(COMMAND "${GIT_EXECUTABLE}" log -n1 --format=%H
        OUTPUT_VARIABLE PROJECT_COMMIT_SHA
        COMMAND_ERROR_IS_FATAL ANY
)
string(STRIP "${PROJECT_COMMIT_SHA}" PROJECT_COMMIT_SHA)
execute_process(COMMAND "${GIT_EXECUTABLE}" tag --points-at=HEAD
        OUTPUT_VARIABLE PROJECT_COMMIT_TAG
        COMMAND_ERROR_IS_FATAL ANY
)
string(STRIP "${PROJECT_COMMIT_TAG}" PROJECT_COMMIT_TAG)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)

add_compile_definitions(QT_NO_KEYWORDS)
include_directories("${PROJECT_SOURCE_DIR}/include" "${PROJECT_BINARY_DIR}/include")

find_package(Qt6 6.7 REQUIRED COMPONENTS
        Core
        Svg
        Widgets
)
qt_standard_project_setup(REQUIRES 6.7)

add_subdirectory(src)
