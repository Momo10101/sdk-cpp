cmake_minimum_required(VERSION 3.14)

include_directories( . )

set(TARGET_NAME symbol.sdk.core)


#catapult_library_target(${TARGET_NAME}
#  crypto
#  io
#  ionet
#  model
#  thread
#  utils
#)


add_subdirectory( crypto )
add_subdirectory( io )
add_subdirectory( ionet )
add_subdirectory( model )
add_subdirectory( thread )
add_subdirectory( utils )

