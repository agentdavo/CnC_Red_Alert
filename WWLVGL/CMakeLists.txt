cmake_minimum_required(VERSION 3.13)

project(lv_ww_lib C ASM)

set(ENABLE_ASM OFF CACHE BOOL "Enable assembly modules")

# Temporary: only build the memory library while other modules are
# converted to C11.
# Build sub-libraries individually
add_subdirectory(mem)
add_subdirectory(AUDIO)
add_subdirectory(PORT)
add_subdirectory(DIPTHONG)
add_subdirectory(DRAWBUFF)
add_subdirectory(EXAMPLE)
add_subdirectory(FONT)
add_subdirectory(IFF)
add_subdirectory(INCLUDE)
add_subdirectory(KEYBOARD)
add_subdirectory(MISC)
add_subdirectory(MONO)
add_subdirectory(MOVIE)
add_subdirectory(PALETTE)
add_subdirectory(PLAYCD)
add_subdirectory(PROFILE)
add_subdirectory(RAWFILE)
add_subdirectory(SHAPE)
add_subdirectory(SRCDEBUG)
add_subdirectory(TILE)
add_subdirectory(TIMER)
add_subdirectory(WINCOMM)
add_subdirectory(WSA)
add_subdirectory(WW_WIN)

set(WWLVGL_SOURCES)

## Legacy globbing disabled during the refactor; removed in favour of
## per-module interface libraries.

add_library(lv_ww_lib INTERFACE)
target_link_libraries(lv_ww_lib INTERFACE
    mem_lib
    audio_lib
    port_lib
    srcdebug_lib
)

target_include_directories(lv_ww_lib INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/INCLUDE
    ${CMAKE_CURRENT_LIST_DIR}/..
    ${CMAKE_CURRENT_LIST_DIR}/../WWFLAT32/WINDOWS
    ${CMAKE_CURRENT_LIST_DIR}/../VQ/INCLUDE/WWLIB32
)

# Enforce strict compilation flags for all sources
set_target_properties(lv_ww_lib PROPERTIES
    C_STANDARD 11
    C_EXTENSIONS ON
)
target_compile_options(lv_ww_lib INTERFACE -std=gnu11 -pedantic -Wall -Wextra -Werror)
target_link_libraries(lv_ww_lib INTERFACE miniaudio)

