cmake_minimum_required (VERSION 2.6)
project (tavla)
add_executable (tavla tavla.c)
install(TARGETS tavla RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
