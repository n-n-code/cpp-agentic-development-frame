function(frame_add_test target_name)
    add_executable(${target_name} ${ARGN})
    target_include_directories(${target_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)
    target_link_libraries(${target_name} PRIVATE project_core)
    add_test(NAME ${target_name} COMMAND ${target_name})
endfunction()

frame_add_test(projectcoretest
    projectcoretest.cpp
)

frame_add_test(clismoketest
    clismoketest.cpp
)

add_test(NAME frameclihelp COMMAND $<TARGET_FILE:frame_cli> --help)

add_test(NAME testcommentarycheck
    COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/../scripts/check-test-commentary.sh ${CMAKE_CURRENT_SOURCE_DIR}/..
)
