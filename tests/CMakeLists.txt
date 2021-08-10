cmake_minimum_required(VERSION 3.14)

include_directories( ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/src )

add_subdirectory(shared)
add_subdirectory(bench)

catapult_test_executable_target(tests.catapult.builders core builders)
catapult_test_executable_target(tests.catapult.extensions core extensions)
catapult_test_executable_target(tests.catapult.parsers core parsers)

target_link_libraries(tests.catapult.parsers
  catapult.extensions
	catapult.plugins.namespace
  )

target_link_libraries(tests.catapult.extensions
	catapult.plugins.namespace
  )

target_link_libraries(tests.catapult.builders
  catapult.extensions
  catapult.plugins.aggregate
  catapult.plugins.mosaic
	catapult.plugins.namespace
  )
