configure_file(xenoprobes_config.h.in ${PROJECT_BINARY_DIR}/include/xenoprobes_config.h)

add_subdirectory(sitelinks)
add_subdirectory(sitelist)
add_subdirectory(probeoptimizer)
add_subdirectory(xenoprobes)
add_subdirectory(xenoprobes_gui)
