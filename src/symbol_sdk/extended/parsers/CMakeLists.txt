cmake_minimum_required(VERSION 3.14)

catapult_library_target(catapult.parsers)
target_link_libraries(catapult.parsers catapult.model catapult.extensions )
