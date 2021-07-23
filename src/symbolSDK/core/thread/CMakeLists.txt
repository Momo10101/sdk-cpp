cmake_minimum_required(VERSION 3.14)

catapult_library_target(symbol.thread)
target_link_libraries(symbol.thread symbol.model symbol.utils)
