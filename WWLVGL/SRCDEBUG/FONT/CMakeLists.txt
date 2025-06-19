cmake_minimum_required(VERSION 3.13)

add_library(srcdebug_font_lib INTERFACE)

target_include_directories(srcdebug_font_lib INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/../INCLUDE
)

target_compile_features(srcdebug_font_lib INTERFACE cxx_std_11)
target_compile_options(srcdebug_font_lib INTERFACE -std=gnu++11)
