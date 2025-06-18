# RISC-V 64IMA bare metal toolchain for QEMU

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR riscv64)

find_program(RISCV_GCC riscv64-unknown-elf-gcc)
find_program(RISCV_GXX riscv64-unknown-elf-g++)

# Fallback to linux-gnu toolchain if the bare metal one is unavailable
if(NOT RISCV_GCC)
    find_program(RISCV_GCC riscv64-linux-gnu-gcc)
    find_program(RISCV_GXX riscv64-linux-gnu-g++)
endif()

set(CMAKE_C_COMPILER ${RISCV_GCC})
set(CMAKE_CXX_COMPILER ${RISCV_GXX})

set(CMAKE_C_FLAGS_INIT "-march=rv64ima -mabi=lp64" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_INIT "-march=rv64ima -mabi=lp64" CACHE STRING "" FORCE)
set(CMAKE_EXE_LINKER_FLAGS_INIT "-nostdlib -static -Wl,--gc-sections" CACHE STRING "" FORCE)

set(ENABLE_ASM OFF CACHE BOOL "Disable assembly" FORCE)
set(USE_C_BLITTERS ON CACHE BOOL "Use C implementations of blitters" FORCE)
set(USE_LVGL ON CACHE BOOL "Enable LVGL")
set(LVGL_BACKEND "x11" CACHE STRING "LVGL backend")

set(QEMU_SYSTEM qemu-system-riscv64 CACHE STRING "QEMU executable")
set(QEMU_FLAGS "-nographic -machine virt -bios none -kernel" CACHE STRING "QEMU flags")

function(add_qemu_run_target target)
    add_custom_target(run
        COMMAND ${QEMU_SYSTEM} ${QEMU_FLAGS} $<TARGET_FILE:${target}>
        DEPENDS ${target}
        USES_TERMINAL
    )
endfunction()
