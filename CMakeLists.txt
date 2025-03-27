cmake_minimum_required (VERSION 3.22)
project (xenoprobes
    VERSION 2.0.0
    DESCRIPTION "Calculate the optimal Data Probe layout for Xenoblade Chronicles X"
    LANGUAGES CXX
    HOMEPAGE_URL "https://github.com/minneyar/xenoprobes"
)
set(PROJECT_DISPLAY_NAME "XenoProbes")
set(PROJECT_ORGANIZATION_NAME "minneyar")
file(READ "${PROJECT_SOURCE_DIR}/AUTHORS" PROJECT_AUTHORS)
# Insert 2 escaped newlines for every newline in the file.
string(REPLACE "\n" "\\n\\n\"\n\"" PROJECT_AUTHORS "${PROJECT_AUTHORS}")
set(PROJECT_AUTHORS_CARR "\"${PROJECT_AUTHORS_CARR}\"")
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

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)

add_compile_definitions(QT_NO_KEYWORDS)
include_directories("${PROJECT_SOURCE_DIR}/include" "${PROJECT_BINARY_DIR}/include")
if (MSVC)
    add_compile_options("/utf-8")
endif ()

find_package(Qt6 6.2 REQUIRED COMPONENTS
        Core
        Svg
        Widgets
)

add_subdirectory(src)
