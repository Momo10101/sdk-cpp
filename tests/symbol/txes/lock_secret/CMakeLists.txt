cmake_minimum_required(VERSION 3.14)

add_executable(tests.catapult.txes.lock_secret
  LockHashUtilsTests.cpp
  SecretLockTransactionTests.cpp
  SecretProofTransactionTests.cpp
  )

target_link_libraries(tests.catapult.txes.lock_secret
  catapult.plugins.lock_secret
  tests.catapult.test.core
  )
