cmake_minimum_required(VERSION 4.1.2)

project(dag-generator)

file(GLOB SRC_FILES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_compile_options(-Wall -Wextra -Wfatal-errors -Wshadow -std=c++20 -O3 -march=native)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    "${PROJECT_SOURCE_DIR}/src"
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}"
)
