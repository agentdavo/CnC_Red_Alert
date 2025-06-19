# DOS build settings
set(CMAKE_SYSTEM_NAME Generic)

# Disable assembly and Windows dependencies
set(ENABLE_ASM OFF CACHE BOOL "Disable assembly" FORCE)
set(USE_LVGL OFF CACHE BOOL "Disable LVGL" FORCE)
set(USE_C_BLITTERS ON CACHE BOOL "Use C blitters" FORCE)

# Sources relying on Windows headers are not compiled
set(PLATFORM_EXCLUDE_SOURCES
    CODE/W95TRACE.CPP
    CODE/MPMGRW.CPP
    CODE/DDE.CPP
    CODE/DIBFILE.CPP
    CODE/DIBUTIL.CPP
)

# No extra sources or libraries on this platform
set(PLATFORM_EXTRA_SOURCES "")
set(PLATFORM_LINK_LIBS "")
