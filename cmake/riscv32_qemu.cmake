# RISC-V 32IMA bare metal toolchain for QEMU

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR riscv32)

find_program(RISCV_GCC riscv32-unknown-elf-gcc)
find_program(RISCV_GXX riscv32-unknown-elf-g++)

# Fallback to 64-bit toolchain with 32-bit flags if riscv32 executables are missing
if(NOT RISCV_GCC)
    find_program(RISCV_GCC riscv64-unknown-elf-gcc)
    find_program(RISCV_GXX riscv64-unknown-elf-g++)
endif()

set(CMAKE_C_COMPILER ${RISCV_GCC})
set(CMAKE_CXX_COMPILER ${RISCV_GXX})

set(CMAKE_C_FLAGS_INIT "-march=rv32ima -mabi=ilp32" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_INIT "-march=rv32ima -mabi=ilp32" CACHE STRING "" FORCE)
set(CMAKE_EXE_LINKER_FLAGS_INIT "-nostdlib -static -Wl,--gc-sections" CACHE STRING "" FORCE)

set(ENABLE_ASM OFF CACHE BOOL "Disable assembly" FORCE)
set(USE_C_BLITTERS ON CACHE BOOL "Use C implementations of blitters" FORCE)
set(USE_LVGL ON CACHE BOOL "Enable LVGL")
set(LVGL_BACKEND "x11" CACHE STRING "LVGL backend")

set(QEMU_SYSTEM qemu-system-riscv32 CACHE STRING "QEMU executable")
set(QEMU_FLAGS "-nographic -machine virt -bios none -kernel" CACHE STRING "QEMU flags")

function(add_qemu_run_target target)
    add_custom_target(run
        COMMAND ${QEMU_SYSTEM} ${QEMU_FLAGS} $<TARGET_FILE:${target}>
        DEPENDS ${target}
        USES_TERMINAL
    )
endfunction()
