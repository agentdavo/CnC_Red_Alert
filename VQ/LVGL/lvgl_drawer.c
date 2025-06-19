#include <stdlib.h>
#include <string.h>
#include "vqaplayp.h"
#include "vq.h"
#include "unvq.h"
#include "../VQM32/compress.h"
#include "../../src/lvgl/src/lvgl.h"
#include "vq_lvgl_player.h"
#include "vq_lvgl_decoder.h"



long PageFlip_Nop(VQAHandle *vqa)
{
    (void)vqa;
    return 0;
}

void UnVQ_Nop(unsigned char *codebook, unsigned char *pointers,
              unsigned char *buffer, unsigned long blocksperrow,
              unsigned long numrows, unsigned long bufwidth)
{
    (void)codebook; (void)pointers; (void)buffer;
    (void)blocksperrow; (void)numrows; (void)bufwidth;
}

static long Select_Frame(VQAHandleP *vqap)
{
    VQAData      *vqabuf = vqap->VQABuf;
    VQADrawer    *drawer = &vqabuf->Drawer;
    VQAConfig    *config = &vqap->Config;
    VQAFrameNode *curframe = drawer->CurFrame;
    long         curtime;
    long         desiredframe;

    if((curframe->Flags & VQAFRMF_LOADED) == 0) {
        drawer->WaitsOnLoader++;
        return VQAERR_NOBUFFER;
    }

    if(config->OptionFlags & VQAOPTF_STEP) {
        drawer->LastFrame = curframe->FrameNum;
        return 0;
    }

    curtime = VQA_GetTime(vqap);
    desiredframe = ((curtime * config->FrameRate) / VQA_TIMETICKS);

    if(config->DrawRate != config->FrameRate) {
        if(curtime - drawer->LastTime < (VQA_TIMETICKS / config->DrawRate))
            return VQAERR_NOT_TIME;
    } else {
        if(curframe->FrameNum > desiredframe)
            return VQAERR_NOT_TIME;
    }

    if((curframe->FrameNum - drawer->LastFrame) >= (config->FrameRate / 5)) {
        drawer->LastFrame = curframe->FrameNum;
        return 0;
    }

    if(config->DrawFlags & VQACFGF_NOSKIP) {
        drawer->LastFrame = curframe->FrameNum;
        return 0;
    }

    while(1) {
        if((curframe->Flags & VQAFRMF_LOADED) == 0)
            return VQAERR_NOBUFFER;

        if(curframe->Flags & VQAFRMF_KEY)
            break;

        if(curframe->FrameNum < desiredframe) {
            if(curframe->Flags & VQAFRMF_PALCOMP) {
                curframe->PaletteSize = LCW_Uncompress((char *)curframe->Palette + curframe->PalOffset,
                                                       (char *)curframe->Palette, vqabuf->Max_Pal_Size);
                curframe->Flags &= ~VQAFRMF_PALCOMP;
            }
            memcpy(drawer->Palette_24, curframe->Palette, curframe->PaletteSize);
            drawer->CurPalSize = curframe->PaletteSize;
            drawer->Flags |= VQADRWF_SETPAL;
            if(config->DrawerCallback) {
                if(config->DrawerCallback(NULL, curframe->FrameNum) != 0)
                    return VQAERR_EOF;
            }
            curframe->Flags = 0L;
            curframe = curframe->Next;
            drawer->CurFrame = curframe;
            drawer->NumSkipped++;
        } else {
            break;
        }
    }

    drawer->LastFrame = curframe->FrameNum;
    drawer->LastTime = curtime;
    return 0;
}

static void Prepare_Frame(VQAData *vqabuf)
{
    VQADrawer    *drawer = &vqabuf->Drawer;
    VQAFrameNode *curframe = drawer->CurFrame;
    VQACBNode    *codebook = curframe->Codebook;

    if(codebook->Flags & VQACBF_CBCOMP) {
        LCW_Uncompress((char *)codebook->Buffer + codebook->CBOffset,
                       (char *)codebook->Buffer, vqabuf->Max_CB_Size);
        codebook->Flags &= ~VQACBF_CBCOMP;
    }

    if(curframe->Flags & VQAFRMF_PALCOMP) {
        curframe->PaletteSize = LCW_Uncompress((char *)curframe->Palette + curframe->PalOffset,
                                              (char *)curframe->Palette, vqabuf->Max_Pal_Size);
        curframe->Flags &= ~VQAFRMF_PALCOMP;
    }

    if(curframe->Flags & VQAFRMF_PTRCOMP) {
        LCW_Uncompress((char *)curframe->Pointers + curframe->PtrOffset,
                       (char *)curframe->Pointers, vqabuf->Max_Ptr_Size);
        curframe->Flags &= ~VQAFRMF_PTRCOMP;
    }
}

long DrawFrame_Buffer(VQAHandle *vqa)
{
    VQAData      *vqabuf = ((VQAHandleP *)vqa)->VQABuf;
    VQADrawer    *drawer = &vqabuf->Drawer;
    VQAFrameNode *curframe;
    VQAConfig    *config = &((VQAHandleP *)vqa)->Config;
    long         rc;
    unsigned char *buff;

    if(!(vqabuf->Flags & VQADATF_DSLEEP)) {
        if((rc = Select_Frame((VQAHandleP *)vqa)) != 0)
            return rc;
        Prepare_Frame(vqabuf);
    }

    if(vqabuf->Flags & VQADATF_UPDATE) {
        vqabuf->Flags |= VQADATF_DSLEEP;
        return VQAERR_SLEEPING;
    }

    if(vqabuf->Flags & VQADATF_DSLEEP) {
        drawer->WaitsOnFlipper++;
        vqabuf->Flags &= ~VQADATF_DSLEEP;
    }

    curframe = drawer->CurFrame;
    buff = (unsigned char *)frame_desc->data + sizeof(vq_raw_image_t);

    vqabuf->UnVQ(curframe->Codebook->Buffer, curframe->Pointers, buff,
                 drawer->BlocksPerRow, drawer->NumRows, drawer->ImageWidth);

    drawer->LastFrameNum = curframe->FrameNum;
    vqabuf->Flipper.CurFrame = curframe;
    vqabuf->Flags |= VQADATF_UPDATE;

    if(config->DrawerCallback) {
        if(config->DrawerCallback((unsigned char *)frame_desc->data + sizeof(vq_raw_image_t), curframe->FrameNum) != 0)
            return VQAERR_EOF;
    }

    drawer->CurFrame = curframe->Next;
    decode_frame_node(vqa, drawer->CurFrame, next_frame_data);
    lv_obj_invalidate(img_obj);
    swap_lvgl_buffers();
    return 0;
}

