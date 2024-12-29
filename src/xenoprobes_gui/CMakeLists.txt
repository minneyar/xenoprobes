qt_add_executable(${PROJECT_NAME}_gui
        ${PROJECT_SOURCE_DIR}/resources/resources.qrc
        DataProbe.cpp
        DataProbe.h
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
        themeIcon.h
        themeIcon.cpp
        main.cpp
)

# Application icon
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_sources(${PROJECT_NAME}_gui PRIVATE xenoprobes.rc)
endif ()

find_package(Boost COMPONENTS container_hash REQUIRED)
find_package(spdlog REQUIRED)
target_link_libraries(${PROJECT_NAME}_gui PRIVATE
        probeoptimizer
        Boost::container_hash
        spdlog::spdlog
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
    if ("${Qt6_VERSION}" VERSION_GREATER_EQUAL "6.3")
        qt_generate_deploy_app_script(
                TARGET "${PROJECT_NAME}_gui"
                OUTPUT_SCRIPT DEPLOY_SCRIPT
                NO_UNSUPPORTED_PLATFORM_ERROR
        )
    else ()
        include(deployqt)
        deployqt(
                TARGET "${PROJECT_NAME}_gui"
                OUTPUT_SCRIPT DEPLOY_SCRIPT
        )
    endif ()
    install(SCRIPT ${DEPLOY_SCRIPT})
endif ()
