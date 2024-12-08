find_package (Threads)
find_package (Boost COMPONENTS system REQUIRED)
find_package(spdlog REQUIRED)

add_library(probeoptimizer
        csv.cpp
        ore.cpp
        probe_arrangement.cpp
        probe.cpp
        probe_optimizer.cpp
        site.cpp
        site_list.cpp
        solution.cpp
        semaphore.cpp
)
target_link_libraries(probeoptimizer PRIVATE
        Boost::system
        spdlog::spdlog
        Threads::Threads
)

# Create site links file.
add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/kAllLinks.cpp"
        COMMAND sitelinks
        "${PROJECT_SOURCE_DIR}/mira.dot"
        "${CMAKE_CURRENT_BINARY_DIR}/kAllLinks.cpp"
        DEPENDS
        "${PROJECT_SOURCE_DIR}/mira.dot"
)
target_sources(probeoptimizer PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/kAllLinks.cpp")
