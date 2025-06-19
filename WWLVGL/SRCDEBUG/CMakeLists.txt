cmake_minimum_required(VERSION 3.13)

add_subdirectory(AUDIO)
add_subdirectory(RAWFILE)
add_subdirectory(PROFILE)
add_subdirectory(SHAPE)
add_subdirectory(WINCOMM)
add_subdirectory(KEYBOARD)
add_subdirectory(MOUSE)
add_subdirectory(TIMER)
add_subdirectory(VESA)
add_subdirectory(FILE)
add_subdirectory(BUFFER)
add_subdirectory(FONT)
add_subdirectory(PALETTE)
add_subdirectory(VIDEO)

add_library(srcdebug_lib INTERFACE)

target_link_libraries(srcdebug_lib INTERFACE
    srcdebug_audio_lib
    srcdebug_rawfile_lib
    srcdebug_profile_lib
    srcdebug_shape_lib
    srcdebug_wincomm_lib
    srcdebug_keyboard_lib
    srcdebug_mouse_lib
    srcdebug_timer_lib
    srcdebug_vesa_lib
    srcdebug_file_lib
    srcdebug_buffer_lib
    srcdebug_font_lib
    srcdebug_palette_lib
    srcdebug_video_lib
)

# expose headers for modules that still include these files directly
# parent include to mirror old layout

# Include parent directories for legacy includes
# (../ refers to WWLVGL)
target_include_directories(srcdebug_lib INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/../INCLUDE
)
