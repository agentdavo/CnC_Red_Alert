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
)

if(ENABLE_ASM)
    list(APPEND VQA32_SOURCES unvq_stub.c)
endif()

# Build the original sources as C++

add_library(vqa32 STATIC ${VQA32_SOURCES})

  target_include_directories(vqa32 PUBLIC
      ${CMAKE_CURRENT_LIST_DIR}
      ${CMAKE_CURRENT_LIST_DIR}/..
      ${CMAKE_CURRENT_LIST_DIR}/../INCLUDE
      ${CMAKE_CURRENT_LIST_DIR}/../VQM32
      "${CMAKE_SOURCE_DIR}"
  )
