find_package (Threads)
find_package (Boost COMPONENTS system REQUIRED)
find_package(spdlog REQUIRED)

add_library(probeoptimizer
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
        $<IF:$<BOOL:MSVC>,,-static>
)
if (MSVC)
    set_target_properties(probeoptimizer PROPERTIES
            MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
    )
    target_compile_options(probeoptimizer PRIVATE
            $<$<CONFIG:>:/MT> #---------|
            $<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
            $<$<CONFIG:Release>:/MT> #--|
    )
endif()
