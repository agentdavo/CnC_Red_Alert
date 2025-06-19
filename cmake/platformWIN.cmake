# Windows build settings
set(CMAKE_SYSTEM_NAME Windows)

set(ENABLE_ASM OFF CACHE BOOL "Disable assembly" FORCE)
set(USE_LVGL OFF CACHE BOOL "Disable LVGL" FORCE)
set(USE_C_BLITTERS ON CACHE BOOL "Use C blitters" FORCE)

# Windows launcher sources and libraries
set(PLATFORM_EXTRA_SOURCES
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
set(PLATFORM_LINK_LIBS ws2_32)

# No exclusions for the Windows build
set(PLATFORM_EXCLUDE_SOURCES "")
