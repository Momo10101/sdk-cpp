cmake_minimum_required(VERSION 3.14)

add_executable(tests.catapult.txes.account_link
  AccountKeyLinkTransactionTests.cpp
  NodeKeyLinkTransactionTests.cpp)

target_link_libraries( tests.catapult.txes.account_link
  tests.catapult.test.core )
