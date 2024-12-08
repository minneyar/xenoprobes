qt_add_executable(${PROJECT_NAME}_gui
        ${PROJECT_SOURCE_DIR}/resources/resources.qrc
        DataProbe.cpp
        DataProbe.h
        FnSite.cpp
        FnSite.h
        FnSiteWidget.cpp
        FnSiteWidget.h
        InventoryLoader.h
        InventoryLoader.cpp
        InventoryModel.h
        InventoryModel.cpp
        MainWindow.cpp
        MainWindow.h
        MiraMap.cpp
        MiraMap.h
        RunOptionsWidget.h
        RunOptionsWidget.cpp
        SiteListLoader.cpp
        SiteListLoader.h
        SliderWithValWidget.h
        SliderWithValWidget.cpp
        SolutionWidget.h
        SolutionWidget.cpp
        SolverRunner.h
        SolverRunner.cpp
        main.cpp
)

# Application icon
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_sources(${PROJECT_NAME}_gui PRIVATE xenoprobes.rc)
endif ()

# Create FN Site data file
add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/kAllSites.cpp"
        COMMAND sitelist
        "${PROJECT_SOURCE_DIR}/sites.csv"
        "${PROJECT_SOURCE_DIR}/resources/map.svg"
        "${CMAKE_CURRENT_BINARY_DIR}/kAllSites.cpp"
        DEPENDS
        "${PROJECT_SOURCE_DIR}/sites.csv"
        "${PROJECT_SOURCE_DIR}/resources/map.svg"
)
target_sources(${PROJECT_NAME}_gui PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/kAllSites.cpp")
# This file needs the header from the source directory.
set_property(SOURCE
        "${CMAKE_CURRENT_BINARY_DIR}/kAllSites.cpp"
        APPEND PROPERTY INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(${PROJECT_NAME}_gui PRIVATE
        probeoptimizer
        Qt::Svg
        Qt::Widgets
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}_gui
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(FILES "$<TARGET_RUNTIME_DLLS:${PROJECT_NAME}_gui>"
        TYPE BIN
)
if (NOT CMAKE_SYSTEM_NAME STREQUAL "Linux")
    qt_generate_deploy_app_script(
            TARGET "${PROJECT_NAME}_gui"
            OUTPUT_SCRIPT DEPLOY_SCRIPT
            NO_UNSUPPORTED_PLATFORM_ERROR
    )
    install(SCRIPT ${DEPLOY_SCRIPT})
endif ()
