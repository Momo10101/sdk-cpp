cmake_minimum_required(VERSION 3.14)

catapult_library_target(symbol.model)
target_link_libraries(symbol.model symbol.crypto)
