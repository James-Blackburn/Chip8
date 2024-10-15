# Define project settings
cmake_minimum_required(VERSION 3.5)
project(Chip8 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXTERNAL_DIR ${PROJECT_SOURCE_DIR}/external)
set(TEST_DIR ${PROJECT_SOURCE_DIR}/tests)

option(BUILD_TESTS "Build tests" ON)

add_subdirectory(external)
add_subdirectory(src)

if (BUILD_TESTS)

    # Ensure GoogleTest is installed on the system
    # Include FetchContent module
    include(FetchContent)

    # Download and configure GoogleTest
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/release-1.11.0.zip
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    include(CTest)
    include(GoogleTest)
    
    add_subdirectory(tests)

endif()

# Build main program
add_executable(chip8 src/main.cpp)
target_link_libraries(chip8 PRIVATE 
    application
    memory 
    display
    shaderProgram
    cpu
    emulator
    keyboard
    speaker
    glfw
    glad
    OpenAL::OpenAL
    imgui
)
set_target_properties(chip8 PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}
)