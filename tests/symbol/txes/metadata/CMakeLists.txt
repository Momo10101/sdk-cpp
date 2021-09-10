cmake_minimum_required(VERSION 3.14)

add_executable(tests.catapult.txes.metadata MetadataTransactionTests.cpp)

target_link_libraries(tests.catapult.txes.metadata tests.catapult.test.core)
