# Add libraries
add_library(memory STATIC emulator/memory.cpp)
target_include_directories(memory PUBLIC ${PROJECT_SOURCE_DIR}/include/emulator/)

add_library(display STATIC emulator/display.cpp)
target_include_directories(display PUBLIC ${PROJECT_SOURCE_DIR}/include/emulator/)

add_library(cpu STATIC emulator/cpu.cpp)
target_include_directories(cpu PUBLIC 
    ${PROJECT_SOURCE_DIR}/include/emulator/
    ${EXTERNAL_DIR}/openal-soft/include/
)

add_library(emulator STATIC emulator/emulator.cpp)
target_include_directories(emulator PUBLIC 
    ${PROJECT_SOURCE_DIR}/include/emulator/
    ${EXTERNAL_DIR}/openal-soft/include/
)

add_library(keyboard STATIC emulator/keyboard.cpp)
target_include_directories(keyboard PUBLIC
    ${PROJECT_SOURCE_DIR}/include/emulator/
    ${EXTERNAL_DIR}/openal-soft/include/
)

add_library(speaker STATIC emulator/speaker.cpp)
target_include_directories(speaker PUBLIC
    ${PROJECT_SOURCE_DIR}/include/emulator/
    ${EXTERNAL_DIR}/openal-soft/include/
)
target_link_libraries(speaker PUBLIC OpenAL::OpenAL)

add_library(shaderProgram STATIC application/shaderProgram.cpp)
target_include_directories(shaderProgram PUBLIC 
    ${PROJECT_SOURCE_DIR}/include/application/
    ${EXTERNAL_DIR}/glad/include/
)

add_library(application STATIC application/application.cpp)
target_include_directories(application PUBLIC
    ${PROJECT_SOURCE_DIR}/include/application/
    ${PROJECT_SOURCE_DIR}/include/emulator/
    ${EXTERNAL_DIR}/glfw/include/
    ${EXTERNAL_DIR}/glad/include/
    ${EXTERNAL_DIR}/openal-soft/include/
    ${EXTERNAL_DIR}/imgui-1.91.3/backends/
    ${EXTERNAL_DIR}/imgui-1.91.3/
)