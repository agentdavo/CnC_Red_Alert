#include <ra/debug_log.h>
#include <string.h>

/*
 * Minimal C replacements for the old assembly helpers used by the
 * Win32 mouse implementation.  These are intentionally simple and only
 * support what the current codebase requires.  The structures mirror the
 * layout from the original assembly include files (GBUFFER.INC and
 * MOUSE.INC) but contain only the fields needed by these helpers.
 */

typedef struct GraphicViewPort {
    unsigned char *GVPOffset;
    int GVPWidth;
    int GVPHeight;
    int GVPXAdd;
    int GVPXPos;
    int GVPYPos;
    int GVPPitch;
    void *GVPBuffPtr;
} GraphicViewPort;

typedef struct MouseType {
    unsigned char *MouseCursor;
    int MouseXHot;
    int MouseYHot;
    int CursorWidth;
    int CursorHeight;
    unsigned char *MouseBuffer;
    int MouseBuffX;
    int MouseBuffY;
    int MaxWidth;
    int MaxHeight;
    /* Remaining fields omitted as the helpers below do not use them. */
} MouseType;

void Mouse_Shadow_Buffer(void *thisptr, void *scrdst, void *buffer,
                         int x, int y, int hotx, int hoty, int store)
{
    MouseType *m = (MouseType *)thisptr;
    GraphicViewPort *vp = (GraphicViewPort *)scrdst;

    if (!m || !vp || !buffer || !vp->GVPOffset)
        return;

    int width  = m->CursorWidth;
    int height = m->CursorHeight;

    unsigned char *dst = vp->GVPOffset +
        (y - hoty) * (vp->GVPPitch + vp->GVPXAdd) +
        (x - hotx);
    unsigned char *buf = (unsigned char *)buffer;

    for (int j = 0; j < height; ++j) {
        if (store)
            memcpy(buf, dst, (size_t)width);
        else
            memcpy(dst, buf, (size_t)width);
        buf += width;
        dst += vp->GVPPitch + vp->GVPXAdd;
    }
}

void Draw_Mouse(void *thisptr, void *scrdst, int x, int y)
{
    MouseType *m = (MouseType *)thisptr;
    GraphicViewPort *vp = (GraphicViewPort *)scrdst;

    if (!m || !vp || !m->MouseCursor || !vp->GVPOffset)
        return;

    int width  = m->CursorWidth;
    int height = m->CursorHeight;

    unsigned char *dst = vp->GVPOffset +
        (y - m->MouseYHot) * (vp->GVPPitch + vp->GVPXAdd) +
        (x - m->MouseXHot);
    unsigned char *src = m->MouseCursor;

    for (int j = 0; j < height; ++j) {
        memcpy(dst, src, (size_t)width);
        src += width;
        dst += vp->GVPPitch + vp->GVPXAdd;
    }
}

void *ASM_Set_Mouse_Cursor(void *thisptr, int hotspotx, int hotspoty, void *cursor)
{
    MouseType *m = (MouseType *)thisptr;
    if (!m)
        return NULL;

    void *prev = m->MouseCursor;
    m->MouseCursor = (unsigned char *)cursor;
    m->MouseXHot = hotspotx;
    m->MouseYHot = hotspoty;

    /* Width and height are unknown without parsing the shape header.  */
    return prev;
}

/* Minimal mouse API used by non-Win32 builds */

int Install_Mouse(int max_width, int max_height, int scr_width, int scr_height)
{
    LOG_CALL("Install_Mouse stub\n");
    (void)max_width; (void)max_height; (void)scr_width; (void)scr_height;
    return 1; /* pretend success */
}

void Reset_Mouse(void)
{
    LOG_CALL("Reset_Mouse stub\n");
}

void Remove_Mouse(void)
{
    LOG_CALL("Remove_Mouse stub\n");
}

int Get_Mouse_State(void)
{
    LOG_CALL("Get_Mouse_State stub\n");
    return 0;
}

int Get_Mouse_X(void)
{
    LOG_CALL("Get_Mouse_X stub\n");
    return 0;
}

int Get_Mouse_Y(void)
{
    LOG_CALL("Get_Mouse_Y stub\n");
    return 0;
}

int Get_Mouse_Disabled(void)
{
    LOG_CALL("Get_Mouse_Disabled stub\n");
    return 0;
}

void *Set_Mouse_Cursor(int xhotspot, int yhotspot, void *cursor)
{
    LOG_CALL("Set_Mouse_Cursor stub\n");
    (void)xhotspot; (void)yhotspot;
    static void *current = NULL;
    void *prev = current;
    current = cursor;
    return prev;
}

void Hide_Mouse(void)
{
    LOG_CALL("Hide_Mouse stub\n");
}

void Show_Mouse(void)
{
    LOG_CALL("Show_Mouse stub\n");
}

void Conditional_Hide_Mouse(int x1, int y1, int x2, int y2)
{
    LOG_CALL("Conditional_Hide_Mouse stub\n");
    (void)x1; (void)y1; (void)x2; (void)y2;
}

void Conditional_Show_Mouse(void)
{
    LOG_CALL("Conditional_Show_Mouse stub\n");
}

