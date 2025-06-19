cmake_minimum_required(VERSION 3.13)
project(redalert C CXX)
enable_testing()

# Platform selection
set(PLATFORM "" CACHE STRING "Build platform (DOS, WIN, LVGL)")
if(PLATFORM)
    string(TOUPPER "${PLATFORM}" PLATFORM)
    include(cmake/platform${PLATFORM}.cmake OPTIONAL)
endif()

include(cmake/base.cmake)


add_subdirectory(CODE)
if(DEFINED PLATFORM_EXCLUDE_SOURCES)
    list(REMOVE_ITEM CODE_SOURCES ${PLATFORM_EXCLUDE_SOURCES})
endif()
add_subdirectory(VQ/VQA32)
add_subdirectory(VQ/LVGL)
set(MINIAUDIO_NO_EXTRA_NODES ON CACHE BOOL "" FORCE)
add_subdirectory(src/miniaudio)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

set(SOURCES
    ${CODE_SOURCES}
    LAUNCH/main.c
    src/miniaudio.c
    src/ddraw/ddraw_stub.c
    src/fast_stub.c
    src/ipx_stub.c
    src/file_io.c
)
if(DEFINED PLATFORM_EXCLUDE_SOURCES)
    list(REMOVE_ITEM SOURCES ${PLATFORM_EXCLUDE_SOURCES})
endif()


list(APPEND SOURCES ${PLATFORM_EXTRA_SOURCES})

set(ASM_SOURCES ${CODE_ASM})

if(NOT ENABLE_ASM)
    set(ASM_SOURCES)
elseif(REPLACEMENT_ASM_DIR)
    file(GLOB ASM_SOURCES "${REPLACEMENT_ASM_DIR}/*.asm")
endif()

add_executable(redalert ${SOURCES})
target_link_libraries(redalert PRIVATE gamecode vqa32 miniaudio pthread)
if(USE_LVGL)
    target_link_libraries(redalert PRIVATE lvgl)
endif()
if(PLATFORM_LINK_LIBS)
    target_link_libraries(redalert PRIVATE ${PLATFORM_LINK_LIBS})
endif()
if(COMMAND add_qemu_run_target)
    add_qemu_run_target(redalert)
endif()
