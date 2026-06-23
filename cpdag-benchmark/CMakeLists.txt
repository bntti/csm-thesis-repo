cmake_minimum_required(VERSION 4.1.2)

project(cpdag-benchmark)

file(GLOB SRC_FILES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/algorithms/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/dag-algorithms/*.cpp"
)

add_compile_options(-Wall -Wextra -Wfatal-errors -O3 -march=native)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    "${PROJECT_SOURCE_DIR}/src"
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}"
)
