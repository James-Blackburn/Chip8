# Add memory test suite
add_executable(test_memory test_memory.cpp)
target_link_libraries(test_memory PRIVATE GTest::gtest_main memory)
gtest_discover_tests(test_memory)

# Copy test files to build directory
add_custom_command(TARGET test_memory POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/tests/test_data/
    ${CMAKE_BINARY_DIR}/tests/test_data/)