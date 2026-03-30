if(NOT TOOL_BIN)
    message(FATAL_ERROR "cppcheck target requested, but cppcheck was not found in PATH")
endif()

if(NOT EXISTS "${BUILD_DIR}/compile_commands.json")
    message(FATAL_ERROR
        "cppcheck target requires ${BUILD_DIR}/compile_commands.json. "
        "Reconfigure with CMAKE_EXPORT_COMPILE_COMMANDS=ON."
    )
endif()

execute_process(
    COMMAND "${TOOL_BIN}"
        --enable=warning,style,performance,portability
        --std=c++23
        --quiet
        --error-exitcode=1
        "--project=${BUILD_DIR}/compile_commands.json"
    RESULT_VARIABLE cppcheck_result
)

if(NOT cppcheck_result EQUAL 0)
    message(FATAL_ERROR "cppcheck reported failures")
endif()
