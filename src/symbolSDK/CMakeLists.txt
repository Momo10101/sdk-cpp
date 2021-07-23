cmake_minimum_required(VERSION 3.14)

#set(TARGET_NAME symbol.sdk)
include_directories( . )

#catapult_library_target(${TARGET_NAME} core extended version)
add_subdirectory( core )
add_subdirectory( extended )
add_subdirectory( version )

