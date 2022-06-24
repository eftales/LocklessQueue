cmake_minimum_required(VERSION 3.16)
project(one)

link_libraries(pthread)

aux_source_directory(.                             SRC)

set(CMAKE_CXX_FLAGS "-g -Wno-write-strings -pipe -O0")

set(CMAKE_CXX_STANDARD 11)

add_executable(one  ${SRC} )
