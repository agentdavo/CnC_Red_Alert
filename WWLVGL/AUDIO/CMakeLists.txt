cmake_minimum_required(VERSION 3.13)

add_library(audio_lib STATIC
    soundio.c
    soundint.c
    soundlck.c
)

# Public include directories
target_include_directories(audio_lib PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/..
    ${CMAKE_CURRENT_LIST_DIR}/../INCLUDE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}
)

# Use strict C11 flags like the rest of WWLVGL
if(MSVC)
    target_compile_options(audio_lib PRIVATE /W4)
else()
    target_compile_options(audio_lib PRIVATE -std=gnu11 -Wall -Wextra -Werror)
endif()

