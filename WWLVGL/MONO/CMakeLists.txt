cmake_minimum_required(VERSION 3.13)

add_library(mono_lib INTERFACE)

target_include_directories(mono_lib INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/..
    ${CMAKE_CURRENT_LIST_DIR}/../INCLUDE
)
