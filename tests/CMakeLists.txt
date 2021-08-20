cmake_minimum_required(VERSION 3.14)

include_directories( ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/src/symbol/extended )

# used to define a catapult int test executable by combining catapult_test_executable and catapult_target
function(catapult_int_test_executable_target TARGET_NAME)
	catapult_test_executable(${TARGET_NAME} ${ARGN})
	catapult_target(${TARGET_NAME})

	set_property(TEST ${TARGET_NAME} PROPERTY LABELS Integration)

	# customize and export compiler options for gtest
	catapult_set_test_compiler_options()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" PARENT_SCOPE)
endfunction()



add_subdirectory(bench)
add_subdirectory(crypto)
add_subdirectory(model)
add_subdirectory(shared)
add_subdirectory(stress)
add_subdirectory(utils)

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
