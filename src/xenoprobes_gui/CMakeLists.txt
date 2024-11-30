find_package(Qt6 6.5 REQUIRED COMPONENTS
        Core
        Widgets
)
qt_standard_project_setup(REQUIRES 6.5)

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
        main.cpp
)

# Create FN Site data file
find_package(Python3 COMPONENTS Interpreter REQUIRED)
add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/kAllSites.cpp"
        COMMAND Python3::Interpreter "${CMAKE_CURRENT_SOURCE_DIR}/fn_sites.py"
        "${PROJECT_SOURCE_DIR}/sites.csv"
        "${PROJECT_SOURCE_DIR}/resources/map.svg"
        "${CMAKE_CURRENT_BINARY_DIR}/kAllSites.cpp"
        DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/fn_sites.py"
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
        Qt::Widgets
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}_gui
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
