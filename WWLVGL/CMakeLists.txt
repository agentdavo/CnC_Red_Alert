cmake_minimum_required(VERSION 3.13)

project(lv_ww_lib C ASM)

set(ENABLE_ASM OFF CACHE BOOL "Enable assembly modules")

# collect C and C++ sources
file(GLOB_RECURSE WWLVGL_C_SOURCES
    AUDIO/*.c DIPTHONG/*.c DRAWBUFF/*.c FONT/*.c IFF/*.c KEYBOARD/*.c
    MEM/*.c MISC/*.c MONO/*.c MOVIE/*.c PALETTE/*.c PLAYCD/*.c PROFILE/*.c
    RAWFILE/*.c SHAPE/*.c SRCDEBUG/*.c TILE/*.c TIMER/*.c WINCOMM/*.c
    WSA/*.c WW_WIN/*.c
)

file(GLOB_RECURSE WWLVGL_CPP_SOURCES
    AUDIO/*.cpp DIPTHONG/*.cpp DRAWBUFF/*.cpp FONT/*.cpp IFF/*.cpp KEYBOARD/*.cpp
    MEM/*.cpp MISC/*.cpp MONO/*.cpp MOVIE/*.cpp PALETTE/*.cpp PLAYCD/*.cpp PROFILE/*.cpp
    RAWFILE/*.cpp SHAPE/*.cpp SRCDEBUG/*.cpp TILE/*.cpp TIMER/*.cpp WINCOMM/*.cpp
    WSA/*.cpp WW_WIN/*.cpp
)

set(WWLVGL_SOURCES ${WWLVGL_C_SOURCES} ${WWLVGL_CPP_SOURCES})

if(ENABLE_ASM)
    file(GLOB_RECURSE WWLVGL_ASM_SOURCES
        AUDIO/*.asm DIPTHONG/*.asm DRAWBUFF/*.asm KEYBOARD/*.asm MEM/*.asm
        MISC/*.asm MONO/*.asm MOVIE/*.asm PALETTE/*.asm PROFILE/*.asm
        SRCDEBUG/*.asm TIMER/*.asm WINCOMM/*.asm WSA/*.asm WW_WIN/*.asm
    )
    set(WWLVGL_SOURCES ${WWLVGL_SOURCES} ${WWLVGL_ASM_SOURCES})
endif()

add_library(lv_ww_lib STATIC ${WWLVGL_SOURCES})

target_include_directories(lv_ww_lib PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/INCLUDE
)

