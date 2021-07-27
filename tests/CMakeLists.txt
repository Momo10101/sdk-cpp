cmake_minimum_required(VERSION 3.14)

set(TARGET_NAME tests.symbol.sdk)
include_directories( ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/src )


add_subdirectory(shared_core)
add_subdirectory(shared_nodeps)

#catapult_test_executable_target(${TARGET_NAME} builders extensions parsers)
catapult_library_target(${TARGET_NAME} builders extensions parsers)
