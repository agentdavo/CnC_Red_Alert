cmake_minimum_required(VERSION 3.13)
project(redalert C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Werror)

option(ENABLE_ASM "Enable assembly modules" ON)
set(REPLACEMENT_ASM_DIR "" CACHE PATH "Directory with replacement assembly code")
add_subdirectory(CODE)

set(SOURCES
    ${CODE_SOURCES}
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
        LAUNCH/main.c
)

set(ASM_SOURCES
${CODE_ASM}
)


if(NOT ENABLE_ASM)
    set(ASM_SOURCES)
elseif(REPLACEMENT_ASM_DIR)
    file(GLOB ASM_SOURCES "${REPLACEMENT_ASM_DIR}/*.asm")
endif()

add_executable(redalert ${SOURCES} ${ASM_SOURCES})
target_link_libraries(redalert PRIVATE gamecode)
