cmake_minimum_required(VERSION 4.1.2)

project(dag-benchmark)

file(GLOB SRC_FILES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/algorithms/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/dag-generators/*.cpp"
)

add_compile_options(-Wall -Wextra -Wfatal-errors -Wshadow -O3 -march=native)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    "${PROJECT_SOURCE_DIR}/src"
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}"
)
