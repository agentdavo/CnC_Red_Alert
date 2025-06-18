#include "debug_log.h"

/* Stub replacements for legacy keyboard and mouse assembly routines. */

void Mouse_Shadow_Buffer(void *thisptr, void *scrdst, void *buffer,
                         int x, int y, int hotx, int hoty, int store)
{
    LOG_CALL("Mouse_Shadow_Buffer stub\n");
    (void)thisptr; (void)scrdst; (void)buffer;
    (void)x; (void)y; (void)hotx; (void)hoty; (void)store;
}

void Draw_Mouse(void *thisptr, void *scrdst, int x, int y)
{
    LOG_CALL("Draw_Mouse stub\n");
    (void)thisptr; (void)scrdst; (void)x; (void)y;
}

void *ASM_Set_Mouse_Cursor(void *thisptr, int hotspotx, int hotspoty, void *cursor)
{
    LOG_CALL("ASM_Set_Mouse_Cursor stub\n");
    (void)thisptr; (void)hotspotx; (void)hotspoty;
    static void *current = NULL;
    void *prev = current;
    current = cursor;
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

