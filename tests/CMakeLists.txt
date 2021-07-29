cmake_minimum_required(VERSION 3.14)

#set(TARGET_NAME tests.catapult.sdk)
include_directories( ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/src )


add_subdirectory(shared_core)
add_subdirectory(shared_nodeps)

#catapult_test_executable_target(${TARGET_NAME} core builders extensions parsers)

catapult_test_executable_target(tests.catapult.builders core builders)
catapult_test_executable_target(tests.catapult.extensions core extensions)
catapult_test_executable_target(tests.catapult.parsers core extensions parsers namespace)

target_link_libraries(tests.catapult.parsers
	catapult.model
  catapult.extensions
#	catapult.state
	catapult.plugins.aggregate
#	catapult.plugins.metadata.sdk
#	catapult.plugins.mosaic.sdk
	catapult.plugins.namespace
#	catapult.plugins.restrictionaccount.sdk
  )


target_link_libraries(tests.catapult.extensions
	catapult.model
  catapult.extensions
  #	catapult.state
  	catapult.plugins.aggregate
  #	catapult.plugins.metadata.sdk
  #	catapult.plugins.mosaic.sdk
	catapult.plugins.namespace
  #	catapult.plugins.restrictionaccount.sdk
  )

target_link_libraries(tests.catapult.builders
	catapult.model
  catapult.extensions
  #	catapult.state
  catapult.plugins.aggregate
  #	catapult.plugins.metadata.sdk
  catapult.plugins.mosaic
	catapult.plugins.namespace
  #	catapult.plugins.restrictionaccount.sdk
  )

#catapult_library_target(${TARGET_NAME} builders extensions parsers)
