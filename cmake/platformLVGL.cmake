# Portable build that relies solely on LVGL for output
set(CMAKE_SYSTEM_NAME Generic)

set(ENABLE_ASM OFF CACHE BOOL "Disable assembly" FORCE)
set(USE_LVGL ON CACHE BOOL "Enable LVGL" FORCE)
set(USE_C_BLITTERS ON CACHE BOOL "Use C blitters" FORCE)

# Exclude Windows dependent sources
set(PLATFORM_EXCLUDE_SOURCES
    CODE/W95TRACE.CPP
    CODE/MPMGRW.CPP
    CODE/DDE.CPP
    CODE/DIBFILE.CPP
    CODE/DIBUTIL.CPP
)

# No extra sources or libraries needed for LVGL platform
set(PLATFORM_EXTRA_SOURCES "")
set(PLATFORM_LINK_LIBS "")
