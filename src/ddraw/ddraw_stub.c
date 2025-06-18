#include "ddraw_stub.h"
#include <stdlib.h>

static HRESULT stub_success(void) { return DD_OK; }

static struct IDirectDraw g_ddraw = {
    .SetCooperativeLevel = (HRESULT (*)(void*, uint32_t))stub_success,
    .SetDisplayMode = (HRESULT (*)(int,int,int))stub_success,
    .CreateSurface = (HRESULT (*)(void*, LPDIRECTDRAWSURFACE*, void*))stub_success,
    .CreatePalette = (HRESULT (*)(uint32_t, PALETTEENTRY*, LPDIRECTDRAWPALETTE*, void*))stub_success,
    .QueryInterface = (HRESULT (*)(const void*, void**))stub_success,
    .Release = (HRESULT (*)(void))stub_success,
    .RestoreDisplayMode = (HRESULT (*)(void))stub_success,
    .GetCaps = (HRESULT (*)(void*, void*))stub_success,
    .WaitForVerticalBlank = (HRESULT (*)(uint32_t, void*))stub_success,
};

HRESULT DirectDrawCreate(void* guid, LPDIRECTDRAW* dd, void* unknown)
{
    (void)guid; (void)unknown;
    if (dd) *dd = &g_ddraw;
    return DD_OK;
}
