#ifndef INPUT_ASM_REPL_H
#define INPUT_ASM_REPL_H

#ifdef __cplusplus
extern "C" {
#endif

void Mouse_Shadow_Buffer(void *thisptr, void *scrdst, void *buffer,
                         int x, int y, int hotx, int hoty, int store);
void Draw_Mouse(void *thisptr, void *scrdst, int x, int y);
void *ASM_Set_Mouse_Cursor(void *thisptr, int hotspotx, int hotspoty, void *cursor);

int Install_Mouse(int max_width, int max_height, int scr_width, int scr_height);
void Reset_Mouse(void);
void Remove_Mouse(void);
int Get_Mouse_State(void);
int Get_Mouse_X(void);
int Get_Mouse_Y(void);
int Get_Mouse_Disabled(void);
void *Set_Mouse_Cursor(int xhotspot, int yhotspot, void *cursor);
void Hide_Mouse(void);
void Show_Mouse(void);
void Conditional_Hide_Mouse(int x1, int y1, int x2, int y2);
void Conditional_Show_Mouse(void);

#ifdef __cplusplus
}
#endif

#endif /* INPUT_ASM_REPL_H */
