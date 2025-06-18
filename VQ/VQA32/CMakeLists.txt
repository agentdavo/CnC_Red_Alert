set(VQA32_SOURCES
    AUDIO.CPP
    CAPTION.CPP
    CONFIG.CPP
    DRAWER.CPP
    DSTREAM.CPP
    LOADER.CPP
    MONODISP.CPP
    TASK.CPP
    VERTAG.CPP
    unvq_stub.c
)

# Build the original sources as C++

add_library(vqa32 STATIC ${VQA32_SOURCES})

  target_include_directories(vqa32 PUBLIC
      ${CMAKE_CURRENT_LIST_DIR}
      ${CMAKE_CURRENT_LIST_DIR}/..
      ${CMAKE_CURRENT_LIST_DIR}/../VQM32
      "${CMAKE_SOURCE_DIR}"
  )
