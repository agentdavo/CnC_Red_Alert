#ifndef DDRAW_STUB_H
#define DDRAW_STUB_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct IDirectDraw IDirectDraw;
typedef struct IDirectDrawSurface IDirectDrawSurface;
typedef struct IDirectDrawPalette IDirectDrawPalette;

typedef IDirectDraw* LPDIRECTDRAW;
typedef IDirectDrawSurface* LPDIRECTDRAWSURFACE;
typedef IDirectDrawPalette* LPDIRECTDRAWPALETTE;
typedef int HRESULT;

typedef struct {
    uint8_t peRed;
    uint8_t peGreen;
    uint8_t peBlue;
    uint8_t peFlags;
} PALETTEENTRY;

#define DD_OK 0

HRESULT DirectDrawCreate(void* guid, LPDIRECTDRAW* dd, void* unknown);

struct IDirectDraw {
    HRESULT (*SetCooperativeLevel)(void* hwnd, uint32_t level);
    HRESULT (*SetDisplayMode)(int w, int h, int bpp);
    HRESULT (*CreateSurface)(void* desc, LPDIRECTDRAWSURFACE* surf, void* unk);
    HRESULT (*CreatePalette)(uint32_t caps, PALETTEENTRY* entries,
                             LPDIRECTDRAWPALETTE* palette, void* unk);
    HRESULT (*QueryInterface)(const void* iid, void** out);
    HRESULT (*Release)(void);
    HRESULT (*RestoreDisplayMode)(void);
    HRESULT (*GetCaps)(void* caps, void* emu);
    HRESULT (*WaitForVerticalBlank)(uint32_t flags, void* event);
};

#ifdef __cplusplus
}
#endif

#endif /* DDRAW_STUB_H */
