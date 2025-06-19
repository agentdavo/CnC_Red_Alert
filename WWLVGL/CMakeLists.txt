cmake_minimum_required(VERSION 3.13)

project(lv_ww_lib C ASM)

set(ENABLE_ASM OFF CACHE BOOL "Enable assembly modules")

# Temporary: only build the memory library while other modules are
# converted to C11.
# Build sub-libraries individually
add_subdirectory(mem)
add_subdirectory(AUDIO)

set(WWLVGL_SOURCES)

# sources that live outside of the component libraries
# Temporarily disabled until the remaining code is fully converted to C11
# file(GLOB extra_c SRCDEBUG/*.c MOVIE/*.c RAWFILE/*.c)
# file(GLOB extra_cpp SRCDEBUG/*.cpp MOVIE/*.cpp RAWFILE/*.cpp)
# list(APPEND WWLVGL_SOURCES ${extra_c} ${extra_cpp})
# if(ENABLE_ASM)
#     file(GLOB extra_asm SRCDEBUG/*.asm MOVIE/*.asm RAWFILE/*.asm)
#     list(APPEND WWLVGL_SOURCES ${extra_asm})
# endif()

add_library(lv_ww_lib INTERFACE)
target_link_libraries(lv_ww_lib INTERFACE mem_lib audio_lib)

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

