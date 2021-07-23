cmake_minimum_required(VERSION 3.14)

include_directories(../../../../external)

catapult_library_target(symbol.crypto)

target_link_libraries(symbol.crypto symbol.utils external)
catapult_add_openssl_dependencies(symbol.crypto)
