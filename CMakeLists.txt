cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(project LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_VERBOSE_MAKEFILE true)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

include_directories(include)
include(Sanitizers.cmake)

# find_package(Catch2 REQUIRED)

# add_executable(test_avl_tree app/test_avl_tree.cpp)

# add_executable(test_map app/test_map.cpp)
# target_link_libraries(test_map Catch2::Catch2)

add_executable(interpreter app/interpreter.cpp)

install(TARGETS interpreter DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(PROGRAMS demo DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(FILES app/test.txt DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/res)