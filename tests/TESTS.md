# Test Programs

## vqa_video_player

A minimal LVGL player that loops the demo VQA files. Build and run:

```bash
cmake -S . -B build -DCMAKE_C_FLAGS="-std=gnu11" -DUSE_LVGL=ON
cmake --build build --target vqa_video_player
./build/tests/vqa_video_player
```

The window is 320x240 and plays each `cc-demo*.vqa` at 15 fps.
