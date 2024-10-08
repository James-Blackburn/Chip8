# Add libraries
add_library(memory STATIC memory.cpp)
target_include_directories(memory PUBLIC ${PROJECT_SOURCE_DIR}/include/)

add_library(display STATIC display.cpp)
target_include_directories(display PUBLIC 
    ${PROJECT_SOURCE_DIR}/include/
    ${EXTERNAL_DIR}/glad/include/
    ${EXTERNAL_DIR}/glfw/include/
)

add_library(shaderProgram STATIC shaderProgram.cpp)
target_include_directories(shaderProgram PUBLIC 
    ${PROJECT_SOURCE_DIR}/include/
    ${EXTERNAL_DIR}/glad/include/
)

add_library(cpu STATIC cpu.cpp)
target_include_directories(cpu PUBLIC 
    ${PROJECT_SOURCE_DIR}/include/
    ${EXTERNAL_DIR}/glad/include/
    ${EXTERNAL_DIR}/glfw/include/
)

add_library(emulator STATIC emulator.cpp)
target_include_directories(emulator PUBLIC 
    ${PROJECT_SOURCE_DIR}/include/
    ${EXTERNAL_DIR}/glad/include/
    ${EXTERNAL_DIR}/glfw/include/
)