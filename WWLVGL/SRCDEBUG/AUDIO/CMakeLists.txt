cmake_minimum_required(VERSION 3.13)

add_library(srcdebug_audio_lib INTERFACE)

target_include_directories(srcdebug_audio_lib INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../INCLUDE
)
