cmake_minimum_required(VERSION 3.13)

add_library(dipthong_lib INTERFACE)

target_include_directories(dipthong_lib INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/..
    ${CMAKE_CURRENT_LIST_DIR}/../INCLUDE
)
