#!/usr/bin/env sh
set -e
script_dir="$(cd "$(dirname "$0")" && pwd)"
repo_root="$(cd "$script_dir/.." && pwd)"
mkdir -p "$repo_root/logs"
cd "$repo_root"
{
    git submodule update --init src/lvgl
    cmake -S . -B build -DPLATFORM=LVGL
    cmake --build build --target lvgl
} > "$repo_root/logs/platformLVGL.log" 2>&1
