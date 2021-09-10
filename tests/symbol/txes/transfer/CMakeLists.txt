cmake_minimum_required(VERSION 3.14)

add_executable(tests.catapult.txes.transfer TransferTransactionTests.cpp)

target_link_libraries(tests.catapult.txes.transfer tests.catapult.test.core)
