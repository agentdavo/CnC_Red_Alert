cmake_minimum_required(VERSION 3.13)

add_library(mem_lib STATIC
    alloc.c
    mem.c
    newdel.c
)

target_compile_definitions(mem_lib PRIVATE WIN32)

# Public header directory
target_include_directories(mem_lib PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/..
    ${CMAKE_CURRENT_LIST_DIR}/../INCLUDE
)

