cmake_minimum_required(VERSION 3.14)

add_executable(tests.catapult.txes.restriction_account AccountRestrictionTransactionTests.cpp)

target_link_libraries(tests.catapult.txes.restriction_account tests.catapult.test.core)
