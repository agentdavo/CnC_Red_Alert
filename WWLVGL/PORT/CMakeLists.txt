cmake_minimum_required(VERSION 3.13)

add_library(port_lib STATIC
    port.c
)

target_include_directories(port_lib PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_SOURCE_DIR}/include/..
    ${CMAKE_CURRENT_LIST_DIR}/../INCLUDE
)

set_target_properties(port_lib PROPERTIES C_STANDARD 11 C_EXTENSIONS ON)


