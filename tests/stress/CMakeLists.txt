cmake_minimum_required(VERSION 3.14)

set(TARGET_NAME tests.catapult.int.stress)

catapult_int_test_executable_target(${TARGET_NAME} test)
target_link_libraries(${TARGET_NAME}
  tests.catapult.test.core
  catapult.extensions
  )

set_property(TEST ${TARGET_NAME} PROPERTY LABELS Stress)
