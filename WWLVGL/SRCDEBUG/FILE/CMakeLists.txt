cmake_minimum_required(VERSION 3.13)

add_library(srcdebug_file_lib INTERFACE)

target_include_directories(srcdebug_file_lib INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../INCLUDE
)

target_compile_features(srcdebug_file_lib INTERFACE cxx_std_11)
target_compile_options(srcdebug_file_lib INTERFACE -std=gnu++11)
