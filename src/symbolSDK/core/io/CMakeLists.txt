cmake_minimum_required(VERSION 3.14)

catapult_library_target(symbol.io)
target_link_libraries(symbol.io symbol.config symbol.model)
