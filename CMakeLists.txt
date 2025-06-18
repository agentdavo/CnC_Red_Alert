cmake_minimum_required(VERSION 3.13)
project(redalert C CXX)

# Build options
option(ENABLE_ASM "Enable assembly modules" ON)
option(USE_LVGL "Enable LVGL canvas output" OFF)
set(REPLACEMENT_ASM_DIR "" CACHE PATH "Directory with replacement assembly code")

if(ENABLE_ASM)
    find_program(ASM_COMPILER NAMES nasm yasm)
    if(NOT ASM_COMPILER)
        message(FATAL_ERROR "NASM or YASM assembler required but not found")
    endif()
    if(WIN32)
        set(ASM_FORMAT win32)
    else()
        set(ASM_FORMAT elf32)
    endif()
endif()

# Compiler configuration
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Werror)

add_subdirectory(CODE)
add_subdirectory(VQ/VQA32)
set(MINIAUDIO_NO_EXTRA_NODES ON CACHE BOOL "" FORCE)
add_subdirectory(src/miniaudio)

set(LAUNCHER_SRC
    LAUNCHER/256bmp.c
    LAUNCHER/bitmap.cpp
    LAUNCHER/configfile.cpp
    LAUNCHER/dialog.cpp
    LAUNCHER/findpatch.cpp
    LAUNCHER/loadbmp.cpp
    LAUNCHER/main.cpp
    LAUNCHER/monod.cpp
    LAUNCHER/patch.cpp
    LAUNCHER/process.cpp
    LAUNCHER/streamer.cpp
    LAUNCHER/wdebug.cpp
    LAUNCHER/winblows.cpp
    LAUNCHER/wstring.cpp
)

set(LAUNCH_SRC
    LAUNCH/main.c
)

set(SOURCES
    ${CODE_SOURCES}
    ${LAUNCHER_SRC}
    ${LAUNCH_SRC}
    src/miniaudio.c
    src/ddraw/ddraw_stub.c
)

set(ASM_SOURCES ${CODE_ASM})

if(NOT ENABLE_ASM)
    set(ASM_SOURCES)
elseif(REPLACEMENT_ASM_DIR)
    file(GLOB ASM_SOURCES "${REPLACEMENT_ASM_DIR}/*.asm")
endif()

add_executable(redalert ${SOURCES})
target_link_libraries(redalert PRIVATE gamecode vqa32 miniaudio pthread)

