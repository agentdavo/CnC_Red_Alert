cmake_minimum_required(VERSION 3.13)

if(USE_LVGL)
    add_library(srcdebug_video_lib
        surface_api.cpp
    )
    target_include_directories(srcdebug_video_lib PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_CURRENT_LIST_DIR}/../INCLUDE
        ${CMAKE_CURRENT_LIST_DIR}/../../src/lvgl/src
    )
    target_compile_features(srcdebug_video_lib PUBLIC cxx_std_11)
    target_compile_options(srcdebug_video_lib PUBLIC -std=gnu++11)
endif()
