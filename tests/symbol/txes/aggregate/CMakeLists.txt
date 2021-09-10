cmake_minimum_required(VERSION 3.14)

add_executable(tests.catapult.txes.aggregate AggregateTransactionTests.cpp)

target_link_libraries( tests.catapult.txes.aggregate
  catapult.plugins.aggregate
  tests.catapult.test.core
  )
