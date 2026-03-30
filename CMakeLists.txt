cmake_minimum_required(VERSION 3.21)

project(cpp_agentic_development_frame VERSION 0.1.0 LANGUAGES CXX)

include(GNUInstallDirs)
include(CTest)

set(FRAME_LICENSE_INSTALL_DIR "${CMAKE_INSTALL_DATADIR}/licenses/${PROJECT_NAME}")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(FRAME_ENABLE_ASAN "Enable AddressSanitizer for repo-owned code" OFF)
option(FRAME_ENABLE_UBSAN "Enable UndefinedBehaviorSanitizer for repo-owned code" OFF)
option(FRAME_ENABLE_COVERAGE "Enable gcov/llvm-cov coverage instrumentation" OFF)
option(FRAME_ENABLE_BENCHMARKS "Build the benchmarks directory" OFF)
option(FRAME_ENABLE_QT "Enable the example Qt-based UI integration path" OFF)
option(FRAME_ENABLE_CLAZY "Enable the clazy analyzer lane for the Qt-based UI path" OFF)

find_package(Doxygen QUIET)

if(FRAME_ENABLE_QT)
    set(CMAKE_AUTOMOC ON)
    find_package(Qt6 REQUIRED COMPONENTS Core Test)
endif()

add_library(project_core STATIC
    src/projectcore.cpp
    src/projectcore.h
)

target_include_directories(project_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_features(project_core PUBLIC cxx_std_23)

add_executable(frame_cli
    src/main.cpp
)

target_link_libraries(frame_cli PRIVATE project_core)
set_target_properties(frame_cli PROPERTIES
    OUTPUT_NAME frame_cli
)

function(frame_enable_sanitizers target_name visibility)
    if(NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
        message(WARNING "Sanitizers were requested, but ${CMAKE_CXX_COMPILER_ID} is not configured for repo-owned sanitizer flags")
        return()
    endif()

    set(sanitizer_compile_flags "")
    set(sanitizer_link_flags "")

    if(FRAME_ENABLE_ASAN)
        list(APPEND sanitizer_compile_flags -fsanitize=address -fno-omit-frame-pointer)
        list(APPEND sanitizer_link_flags -fsanitize=address)
    endif()

    if(FRAME_ENABLE_UBSAN)
        list(APPEND sanitizer_compile_flags -fsanitize=undefined)
        list(APPEND sanitizer_link_flags -fsanitize=undefined)
    endif()

    if(sanitizer_compile_flags)
        target_compile_options(${target_name} ${visibility} ${sanitizer_compile_flags})
        target_link_options(${target_name} ${visibility} ${sanitizer_link_flags})
    endif()
endfunction()

frame_enable_sanitizers(project_core PUBLIC)
frame_enable_sanitizers(frame_cli PRIVATE)

function(frame_enable_coverage target_name visibility)
    if(NOT FRAME_ENABLE_COVERAGE)
        return()
    endif()
    if(NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
        message(WARNING "Coverage requested but ${CMAKE_CXX_COMPILER_ID} is not supported")
        return()
    endif()
    target_compile_options(${target_name} ${visibility} --coverage -fprofile-arcs -ftest-coverage)
    target_link_options(${target_name} ${visibility} --coverage)
endfunction()

frame_enable_coverage(project_core PUBLIC)
frame_enable_coverage(frame_cli PRIVATE)

set(FRAME_CLAZY_CHECKS "level0" CACHE STRING "Checks passed to clazy-standalone")
find_program(FRAME_CLANG_TIDY_BIN NAMES clang-tidy)
find_program(FRAME_CLAZY_BIN NAMES clazy-standalone)

add_custom_target(clang-tidy
    COMMAND ${CMAKE_COMMAND}
        -DTOOL_BIN=${FRAME_CLANG_TIDY_BIN}
        -DTOOL_NAME=clang-tidy
        -DBUILD_DIR=${CMAKE_BINARY_DIR}
        -DSOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}
        -DCONFIG_FILE=${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy
        -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/run-clang-tidy.cmake
    USES_TERMINAL
    VERBATIM
)

add_custom_target(clazy
    COMMAND ${CMAKE_COMMAND}
        -DTOOL_BIN=${FRAME_CLAZY_BIN}
        -DTOOL_NAME=clazy-standalone
        -DBUILD_DIR=${CMAKE_BINARY_DIR}
        -DSOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}
        -DCHECKS=${FRAME_CLAZY_CHECKS}
        -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/run-clazy.cmake
    USES_TERMINAL
    VERBATIM
)

add_custom_target(lint)
add_dependencies(lint clang-tidy)
if(FRAME_ENABLE_CLAZY)
    add_dependencies(lint clazy)
endif()

add_custom_target(valgrind
    COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/scripts/run-valgrind.sh ${CMAKE_BINARY_DIR}
    USES_TERMINAL
    VERBATIM
)

find_program(FRAME_CLANG_FORMAT_BIN NAMES clang-format)
if(FRAME_CLANG_FORMAT_BIN)
    file(GLOB_RECURSE FRAME_FORMAT_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/benchmarks/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/benchmarks/*.h
    )

    add_custom_target(format
        COMMAND ${FRAME_CLANG_FORMAT_BIN} -i ${FRAME_FORMAT_SOURCES}
        COMMENT "Running clang-format on repo-owned sources"
        VERBATIM
    )

    add_custom_target(format-check
        COMMAND ${FRAME_CLANG_FORMAT_BIN} --dry-run --Werror ${FRAME_FORMAT_SOURCES}
        COMMENT "Checking clang-format compliance"
        USES_TERMINAL
        VERBATIM
    )
else()
    message(STATUS "clang-format not found; format and format-check targets will be unavailable")
endif()

find_program(FRAME_CPPCHECK_BIN NAMES cppcheck)
add_custom_target(cppcheck
    COMMAND ${CMAKE_COMMAND}
        -DTOOL_BIN=${FRAME_CPPCHECK_BIN}
        -DTOOL_NAME=cppcheck
        -DBUILD_DIR=${CMAKE_BINARY_DIR}
        -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/run-cppcheck.cmake
    COMMENT "Running cppcheck analysis"
    USES_TERMINAL
    VERBATIM
)
add_dependencies(lint cppcheck)

if(FRAME_ENABLE_COVERAGE)
    find_program(FRAME_GCOVR_BIN NAMES gcovr)
    add_custom_target(coverage
        COMMAND ${CMAKE_CTEST_COMMAND} --test-dir ${CMAKE_BINARY_DIR} --output-on-failure
        COMMAND ${CMAKE_COMMAND} -E echo "---"
        COMMAND ${CMAKE_COMMAND} -E echo "Collecting coverage data..."
        COMMAND ${FRAME_GCOVR_BIN}
            --root ${CMAKE_CURRENT_SOURCE_DIR}
            --filter ${CMAKE_CURRENT_SOURCE_DIR}/src/
            --print-summary
            --html-details ${CMAKE_BINARY_DIR}/coverage/index.html
        COMMENT "Running tests and collecting coverage"
        USES_TERMINAL
        VERBATIM
    )
endif()

if(DOXYGEN_FOUND)
    set(FRAME_DOXYGEN_OUTPUT_DIR "${CMAKE_BINARY_DIR}/docs/doxygen")
    set(FRAME_DOXYGEN_CONFIG "${CMAKE_BINARY_DIR}/Doxyfile")
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in
        ${FRAME_DOXYGEN_CONFIG}
        @ONLY
    )

    add_custom_target(docs
        COMMAND ${CMAKE_COMMAND} -E make_directory ${FRAME_DOXYGEN_OUTPUT_DIR}
        COMMAND ${DOXYGEN_EXECUTABLE} ${FRAME_DOXYGEN_CONFIG}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
else()
    message(STATUS "Doxygen not found; the docs target will be unavailable")
endif()

install(TARGETS frame_cli RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES LICENSE DESTINATION ${FRAME_LICENSE_INSTALL_DIR})
install(FILES contrib/example-app.service DESTINATION ${CMAKE_INSTALL_DATADIR}/examples/${PROJECT_NAME})
install(FILES contrib/org.example.project.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

if(FRAME_ENABLE_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()
