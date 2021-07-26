cmake_minimum_required(VERSION 3.14)

set(TARGET_NAME tests.symbol.sdk)
include_directories( ../src/ )

catapult_test_executable_target(${TARGET_NAME} builders extensions parsers shared)

