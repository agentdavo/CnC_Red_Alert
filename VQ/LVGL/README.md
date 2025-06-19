# LVGL VQA Library

This directory contains a variant of the Westwood `vqa32` player that outputs
frames to [LVGL](https://lvgl.io). `lvgl_drawer.c` implements drawing into an
`lv_img_dsc_t` which is swapped each frame.

The player exposes `lvgl_drawer_init` and helpers for decoding frames into a raw
image descriptor stored in `frame_desc`【F:VQ/LVGL/vq_lvgl_player.c†L8-L84】.
`VQA_Configure_Drawer` initializes these buffers and sets the decode callbacks
when `USE_LVGL` is enabled【F:VQ/LVGL/LOADER.c†L2852-L2891】.

See `demos/` for sample VQA files.
