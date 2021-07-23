cmake_minimum_required(VERSION 3.14)

catapult_library_target(symbol.ionet)
target_link_libraries(symbol.ionet symbol.model symbol.thread)
catapult_add_openssl_dependencies(symbol.ionet)
