
# Command & Conquer Red Alert

This repository includes source code for Command & Conquer Red Alert. This release provides support to the [Steam Workshop](https://steamcommunity.com/workshop/browse/?appid=2229840) for the game.


## Dependencies

If you wish to rebuild the source code and tools successfully you will need to find or write new replacements (or remove the code using them entirely) for the following libraries;

- DirectX 5 SDK
- DirectX Media 5.1 SDK
- Greenleaf Communications Library (GCL)
- Human Machine Interface (HMI) “Sound Operating System” (SOS)


## Compiling (Win32 Only)

The current state of the source code does not fully compile and will require some effort to restore it. If you wish to restore the original build environment, the following tools are required;

- Watcom C/C++ (v10.6) for C/C++ source files
- Borland Turbo Assembler (TASM v4.0) for assembly files

To use the compiled binaries, you must own the game. The C&C Ultimate Collection is available for purchase on [EA App](https://www.ea.com/en-gb/games/command-and-conquer/command-and-conquer-the-ultimate-collection/buy/pc) or [Steam](https://store.steampowered.com/bundle/39394/Command__Conquer_The_Ultimate_Collection/).


## Contributing

This repository will not be accepting contributions (pull requests, issues, etc). If you wish to create changes to the source code and encourage collaboration, please create a fork of the repository under your GitHub user/organization space.


## Support

This repository is for preservation purposes only and is archived without support. 


## License

This repository and its contents are licensed under the GPL v3 license, with additional terms applied. Please see [LICENSE.md](LICENSE.md) for details.

## Entry points

The DOS launcher begins execution at the `Start` label in `LAUNCH/launch.asm`. In the portable version this is exposed as `launch_main` in `LAUNCH/main.c`. Windows builds start in the standard Win32 `WinMain` located in `CODE/STARTUP.CPP`.

## Graphics modes

Rendering can happen through three different routines depending on the platform and resolution:

- **DirectDraw** – used for 640×400 or higher resolutions (see `DDRAW.md`).
- **ModeX_Blit** – copies 320×200 buffers in VGA Mode X (`MODEX.md`).
- **Shadow_Blit** – DOS-specific VGA blit (`SHADOWX.md`).

## LVGL canvas output

An experimental LVGL canvas can replace DirectDraw during the launcher.
Fetch the submodule and compile with the option enabled.
The launcher reads the `LV_BACKEND` environment variable at runtime and
defaults to `x11`.  Available backend names are `x11`, `wayland`,
`fbdev`, and `sdl`:

```sh
git submodule update --init src/lvgl
cp src/lvgl/lv_conf_template.h lv_conf.h    # basic configuration
cmake -S . -B build -DCMAKE_C_FLAGS="-std=gnu11" -DUSE_LVGL=ON -DLVGL_BACKEND=$LV_BACKEND
cmake --build build
./build/redalert --lvgl-backend=wayland
```

You may omit `--lvgl-backend` and rely on the `LV_BACKEND` environment variable.

The launcher boots into the first menu using the selected LVGL backend.
Pass `--lvgl-backend` to override `LV_BACKEND` at runtime.

With `USE_LVGL` enabled the hidden page is copied to the LVGL canvas via the
`lvgl_blit` routine inside `GScreenClass::Blit_Display`.

## Quick build

A minimal CMake setup is provided for early testing. Run:

```sh
cmake -S . -B build -DCMAKE_C_FLAGS="-std=gnu11"
cmake --build build
```

The build requires **nasm** (or *yasm*) when `ENABLE_ASM` is set.

Compilation currently fails because of missing dependencies and obsolete pragmas, but the commands illustrate the expected process.


## Input devices

- [KEYBOARD.md](KEYBOARD.md) – keyboard queue and LVGL keyboard notes.
- [MOUSE.md](MOUSE.md) – mouse handler and LVGL input device notes.
The input driver defaults to the **x11** backend. Set `LV_BACKEND`
before running the launcher or use `-DLVGL_BACKEND=<name>` when
configuring CMake, or pass `--lvgl-backend <name>` on the command line to select
another backend.
