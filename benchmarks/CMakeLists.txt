add_executable(example_bench example_bench.cpp)
target_include_directories(example_bench PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
)
target_link_libraries(example_bench PRIVATE project_core)
frame_enable_sanitizers(example_bench PRIVATE)
frame_enable_coverage(example_bench PRIVATE)
