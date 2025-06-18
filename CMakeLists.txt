cmake_minimum_required(VERSION 3.13)
project(redalert C CXX)

# Build options
option(ENABLE_ASM "Enable assembly modules" ON)
set(REPLACEMENT_ASM_DIR "" CACHE PATH "Directory with replacement assembly code")

# Compiler configuration
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Werror)

add_subdirectory(CODE)

# Launcher sources
file(GLOB LAUNCHER_SRC
    LAUNCHER/*.c
    LAUNCHER/*.cpp
    LAUNCHER/util/*.cpp
    LAUNCHER/util/*.c)

# Launch stub
file(GLOB LAUNCH_SRC LAUNCH/*.c)

set(SOURCES ${CODE_SOURCES} ${LAUNCHER_SRC} ${LAUNCH_SRC})
set(ASM_SOURCES ${CODE_ASM})

if(NOT ENABLE_ASM)
    set(ASM_SOURCES)
elseif(REPLACEMENT_ASM_DIR)
    file(GLOB ASM_SOURCES "${REPLACEMENT_ASM_DIR}/*.asm")
endif()

add_executable(redalert ${SOURCES} ${ASM_SOURCES})
target_link_libraries(redalert PRIVATE gamecode)
target_compile_features(redalert PRIVATE c_std_11)
