# Add libraries
add_library(memory STATIC memory.cpp)
target_include_directories(memory PUBLIC ${PROJECT_SOURCE_DIR}/include/)