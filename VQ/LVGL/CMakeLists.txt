set(VQLVGL_SOURCES
    AUDIO.CPP
    CAPTION.CPP
    CONFIG.CPP
    DRAWER.CPP
    DSTREAM.CPP
    LOADER.CPP
    MONODISP.CPP
    TASK.CPP
    VERTAG.CPP
)

# Build the LVGL variant of the VQA player using the DOS sources.
add_library(vqa32_lvgl STATIC ${VQLVGL_SOURCES})

# Require C++11 for the playback library
target_compile_features(vqa32_lvgl PUBLIC cxx_std_11)

# Public include paths match the DOS version
target_include_directories(vqa32_lvgl PUBLIC
     ${CMAKE_CURRENT_LIST_DIR}
     ${CMAKE_CURRENT_LIST_DIR}/..
     ${CMAKE_CURRENT_LIST_DIR}/../INCLUDE
     ${CMAKE_CURRENT_LIST_DIR}/../VQM32
     "${CMAKE_SOURCE_DIR}"
 )

# Enable LVGL output and C blitters while leaving assembly disabled
target_compile_definitions(vqa32_lvgl PUBLIC USE_LVGL)
