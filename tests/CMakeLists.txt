cmake_minimum_required(VERSION 3.14)

include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/external ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/src/symbol/extended)

add_subdirectory(bench)
add_subdirectory(symbol)
add_subdirectory(shared)
add_subdirectory(stress)
