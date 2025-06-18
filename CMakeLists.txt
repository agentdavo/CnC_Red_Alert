cmake_minimum_required(VERSION 3.16)

# Basic project setup
project(RedAlert C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

# Enable warnings while the code is being modernized
add_compile_options(-Wall -Wextra)

# Gather source files
file(GLOB_RECURSE CODE_SOURCES CONFIGURE_DEPENDS
    CODE/*.c
    CODE/*.cpp
)
file(GLOB_RECURSE LAUNCHER_SOURCES CONFIGURE_DEPENDS
    LAUNCHER/*.c
    LAUNCHER/*.cpp
)
file(GLOB_RECURSE LAUNCH_SOURCES CONFIGURE_DEPENDS
    LAUNCH/*.c
)

set(SOURCES
    CODE/STARTUP.CPP
    ${CODE_SOURCES}
    ${LAUNCHER_SOURCES}
    ${LAUNCH_SOURCES}
    src/miniaudio.c
)

# Placeholders for external dependencies that still rely on
# DirectX, HMI audio and other legacy libraries.
# TODO: replace these with portable equivalents or stubs.
set(EXTERNAL_LIBS
    # DirectX libraries
    # HMI SOS library
)

set(MINIAUDIO_NO_EXTRA_NODES ON CACHE BOOL "" FORCE)
add_subdirectory(src/miniaudio)

add_executable(redalert ${SOURCES})

target_link_libraries(redalert PRIVATE miniaudio ${EXTERNAL_LIBS} pthread)
