#include "../CODE/lvgl/lvgl_backend.h"
#include "../VQ/LVGL/INCLUDE/vqaplay.h"
#include "../VQ/LVGL/vq_lvgl_player.h"
#include "../CODE/externs.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../src/lvgl/src/lvgl.h"

int main(void) {
    lv_init();
    ScreenWidth = 320;
    ScreenHeight = 240;
    if(lvgl_init_backend("x11") != 0) {
        fprintf(stderr, "LVGL backend init failed\n");
        return 1;
    }

    const char *files[] = {
        "VQ/LVGL/demos/cc-demo1.vqa",
        "VQ/LVGL/demos/cc-demo2.vqa",
        "VQ/LVGL/demos/cc-demo3.vqa",
        "VQ/LVGL/demos/cc-demo4.vqa"
    };
    int num = sizeof(files)/sizeof(files[0]);

    VQAHandle *handle = VQA_Alloc();
    VQA_InitAsDOS(handle);
    VQAConfig cfg;
    VQA_DefaultConfig(&cfg);
    cfg.DrawRate = 15;
    cfg.ImageWidth = 320;
    cfg.ImageHeight = 240;
    cfg.OptionFlags = 0;
    cfg.DrawFlags |= VQACFGF_BUFFER;

    while(1) {
        for(int i=0;i<num;i++) {
            if(VQA_Open(handle, files[i], &cfg) != 0) continue;
            while(VQA_Play(handle, VQAMODE_RUN) != VQAERR_EOF) {
                lv_timer_handler();
                usleep(1000000 / cfg.DrawRate);
            }
            VQA_Close(handle);
        }
    }
    VQA_Free(handle);
    return 0;
}
