cmake_minimum_required(VERSION 3.14)

add_executable(tests.catapult.txes.lock_hash HashLockTransactionTests.cpp)

target_link_libraries(tests.catapult.txes.lock_hash tests.catapult.test.core)
