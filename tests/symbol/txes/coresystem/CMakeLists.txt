cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 17)

add_executable(tests.catapult.txes.coresystem
  VotingKeyLinkTransactionTests.cpp
  VrfKeyLinkTransactionTests.cpp
  )

target_link_libraries(tests.catapult.txes.coresystem
  tests.catapult.test.core
  )
