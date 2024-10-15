# Add memory test suite
add_executable(test_memory test_memory.cpp)
target_link_libraries(test_memory PRIVATE GTest::gtest_main memory)
gtest_discover_tests(test_memory)

# Add display test suite
add_executable(test_display test_display.cpp)
target_link_libraries(test_display PRIVATE GTest::gtest_main display)
gtest_discover_tests(test_display)

# Add cpu test suite
add_executable(test_cpu test_cpu.cpp)
target_link_libraries(test_cpu PRIVATE 
    GTest::gtest_main 
    cpu
    memory
    display
    emulator
    keyboard
    speaker
)
gtest_discover_tests(test_cpu)

# Copy resource files to test build directory
add_custom_command(TARGET test_memory POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/res/
    ${CMAKE_BINARY_DIR}/tests/res/
)