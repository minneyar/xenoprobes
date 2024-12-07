cmake_minimum_required (VERSION 3.28)
project (xenoprobes
    VERSION 1.0.0
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)

add_compile_definitions(QT_NO_KEYWORDS)
include_directories("${PROJECT_SOURCE_DIR}/include" "${PROJECT_BINARY_DIR}/include")

find_package(Qt6 6.5 REQUIRED COMPONENTS
        Core
        Svg
        Widgets
)
qt_standard_project_setup(REQUIRES 6.5)

add_subdirectory(src)
