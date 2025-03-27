# Calls windeployqt or macdeployqt as needed.
function(deployqt)
    include(GNUInstallDirs)
    find_package(Qt6 6.2 REQUIRED COMPONENTS Tools)
    cmake_parse_arguments(PARSE_ARGV 0 "ARG" "" "TARGET;OUTPUT_SCRIPT" "")

    # Create deploy command and args.
    if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
        get_property(DEPLOY_CMD TARGET Qt6::windeployqt PROPERTY IMPORTED_LOCATION)
        list(APPEND DEPLOY_CMD_ARGS "${CMAKE_INSTALL_BINDIR}/$<TARGET_FILE_NAME:${ARG_TARGET}>")
    elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        get_property(DEPLOY_CMD TARGET Qt6::macdeployqt PROPERTY IMPORTED_LOCATION)
        list(APPEND DEPLOY_CMD_ARGS "$<TARGET_BUNDLE_DIR_NAME:${ARG_TARGET}>")
    endif ()
    list(JOIN DEPLOY_CMD_ARGS "\" \"" DEPLOY_CMD_ARGS)

    # Write output script
    set(OUTPUT_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/install_${ARG_TARGET}.cmake")
    file(
            GENERATE OUTPUT "${OUTPUT_SCRIPT}"
            CONTENT "execute_process(COMMAND \"${DEPLOY_CMD}\" \"${DEPLOY_CMD_ARGS}\" WORKING_DIRECTORY \"\${CMAKE_INSTALL_PREFIX}\")"
            TARGET "${ARG_TARGET}"
    )

    # return
    set("${ARG_OUTPUT_SCRIPT}" "${OUTPUT_SCRIPT}" PARENT_SCOPE)
endfunction()
