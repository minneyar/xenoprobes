find_package (Boost COMPONENTS filesystem program_options REQUIRED)

add_executable(${PROJECT_NAME}
        main.cpp
)
target_link_libraries(${PROJECT_NAME} PRIVATE
        probeoptimizer
        Boost::filesystem
        Boost::program_options
        $<IF:$<BOOL:MSVC>,,-static>
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
