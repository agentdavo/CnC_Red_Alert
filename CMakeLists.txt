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
        LAUNCHER/256BMP.C
        LAUNCHER/BITMAP.CPP
        LAUNCHER/CONFIGFILE.CPP
        LAUNCHER/DIALOG.CPP
        LAUNCHER/FINDPATCH.CPP
        LAUNCHER/LOADBMP.CPP
        LAUNCHER/MAIN.CPP
        LAUNCHER/MONOD.CPP
        LAUNCHER/PATCH.CPP
        LAUNCHER/PROCESS.CPP
        LAUNCHER/STREAMER.CPP
        LAUNCHER/WDEBUG.CPP
        LAUNCHER/WINBLOWS.CPP
        LAUNCHER/WSTRING.CPP
)

set(ASM_SOURCES
${CODE_ASM}
LAUNCH/LAUNCH.ASM
)


if(NOT ENABLE_ASM)
    set(ASM_SOURCES)
elseif(REPLACEMENT_ASM_DIR)
    file(GLOB ASM_SOURCES "${REPLACEMENT_ASM_DIR}/*.asm")
endif()

add_executable(redalert ${SOURCES} ${ASM_SOURCES})
target_link_libraries(redalert PRIVATE gamecode)
