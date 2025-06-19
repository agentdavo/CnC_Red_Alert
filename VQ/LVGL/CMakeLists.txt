set(VQLVGL_SOURCES
    AUDIO.c
    CAPTION.c
    CONFIG.c
    lvgl_drawer.c
    vq_lvgl_player.c
    vq_lvgl_decoder.c
    DSTREAM.c
    LOADER.c
    MONODISP.c
    TASK.c
    VERTAG.c
    unvq_stub.c
)

# Build the LVGL variant of the VQA player using the DOS sources.
add_library(vqa32_lvgl STATIC ${VQLVGL_SOURCES})



# Public include paths match the DOS version
target_include_directories(vqa32_lvgl PUBLIC
     ${CMAKE_CURRENT_LIST_DIR}
     ${CMAKE_CURRENT_LIST_DIR}/INCLUDE
     ${CMAKE_CURRENT_LIST_DIR}/..
     ${CMAKE_CURRENT_LIST_DIR}/../INCLUDE
    ${CMAKE_CURRENT_LIST_DIR}/../VQM32
    "${CMAKE_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/WWFLAT32/INCLUDE"
 )

# Enable LVGL output and C blitters while leaving assembly disabled
target_compile_definitions(vqa32_lvgl PUBLIC USE_LVGL USE_C_BLITTERS)
