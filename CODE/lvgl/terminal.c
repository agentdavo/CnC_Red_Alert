#include "terminal.h"
#include "lvgl/lvgl.h"
#include <string.h>

#define TERMINAL_ANIM_TIME   100
#define TERMINAL_LOG_LENGTH  512

static lv_obj_t * win;
static char txt_log[TERMINAL_LOG_LENGTH + 1];
static lv_obj_t * label;
static lv_obj_t * clr_btn;

static lv_res_t clr_click_action(lv_obj_t * btn);
static lv_res_t win_close_action(lv_obj_t * btn);

lv_obj_t * terminal_create(void)
{
    static lv_style_t style_bg;
    lv_style_copy(&style_bg, &lv_style_pretty);
    style_bg.body.main_color = LV_COLOR_MAKE(0x30, 0x30, 0x30);
    style_bg.body.grad_color = LV_COLOR_MAKE(0x30, 0x30, 0x30);
    style_bg.body.border.color = LV_COLOR_WHITE;
    style_bg.text.color = LV_COLOR_MAKE(0xE0, 0xE0, 0xE0);

    win = lv_win_create(lv_scr_act(), NULL);
    lv_win_set_style(win, LV_WIN_STYLE_BG, &style_bg);
    lv_win_set_sb_mode(win, LV_SB_MODE_AUTO);
    lv_win_add_btn(win, LV_SYMBOL_CLOSE, win_close_action);
    lv_win_set_layout(win, LV_LAYOUT_PRETTY);

    label = lv_label_create(win, NULL);
    lv_label_set_long_mode(label, LV_LABEL_LONG_BREAK);
    lv_obj_set_width(label, lv_win_get_width(win));
    lv_label_set_static_text(label, txt_log);

    clr_btn = lv_btn_create(win, NULL);
    lv_cont_set_fit(clr_btn, true, true);
    lv_btn_set_action(clr_btn, LV_BTN_ACTION_CLICK, clr_click_action);
    lv_obj_t * btn_label = lv_label_create(clr_btn, NULL);
    lv_label_set_text(btn_label, "Clear");

    return win;
}

void terminal_add(const char * txt_in)
{
    if(win == NULL) return;

    uint16_t txt_len = strlen(txt_in);
    uint16_t old_len = strlen(txt_log);

    if(txt_len > TERMINAL_LOG_LENGTH) {
        txt_in += (txt_len - TERMINAL_LOG_LENGTH);
        txt_len = TERMINAL_LOG_LENGTH;
        old_len = 0;
    }
    else if(old_len + txt_len > TERMINAL_LOG_LENGTH) {
        uint16_t new_start;
        for(new_start = 0; new_start < old_len; new_start++) {
            if(txt_log[new_start] == '\n') {
                if(new_start >= txt_len) {
                    while(txt_log[new_start] == '\n' || txt_log[new_start] == '\r') new_start++;
                    break;
                }
            }
        }
        if(new_start == old_len) {
            new_start = old_len - (TERMINAL_LOG_LENGTH - txt_len);
        }
        for(uint16_t j = new_start; j < old_len; j++) {
            txt_log[j - new_start] = txt_log[j];
        }
        old_len = old_len - new_start;
        txt_log[old_len] = '\0';
    }

    memcpy(&txt_log[old_len], txt_in, txt_len);
    txt_log[old_len + txt_len] = '\0';

    lv_label_set_static_text(label, txt_log);
    lv_win_focus(win, clr_btn, TERMINAL_ANIM_TIME);
}

static lv_res_t clr_click_action(lv_obj_t * btn)
{
    (void)btn;
    txt_log[0] = '\0';
    lv_label_set_static_text(label, txt_log);
    return LV_RES_OK;
}

static lv_res_t win_close_action(lv_obj_t * btn)
{
    (void)btn;
    lv_obj_del(win);
    win = NULL;
    return LV_RES_INV;
}

