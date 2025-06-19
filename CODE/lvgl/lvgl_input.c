#include "lvgl_input.h"
#include "../../src/lvgl/src/lvgl.h"
#include "../../src/debug_log.h"

#define KEY_QUEUE_LEN 32

struct key_event {
    uint32_t key;
    lv_indev_state_t state;
};

static struct key_event key_queue[KEY_QUEUE_LEN];
static int key_head;
static int key_tail;

static int16_t mouse_x;
static int16_t mouse_y;
static lv_indev_state_t mouse_state;

static lv_indev_t *key_indev;
static lv_indev_t *mouse_indev;

static void keyboard_read(lv_indev_t *indev, lv_indev_data_t *data)
{
    (void)indev;
    if(key_head != key_tail) {
        *data = (lv_indev_data_t){0};
        data->state = key_queue[key_head].state;
        data->key = key_queue[key_head].key;
        key_head = (key_head + 1) % KEY_QUEUE_LEN;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
        data->key = 0;
    }
}

static void mouse_read(lv_indev_t *indev, lv_indev_data_t *data)
{
    (void)indev;
    data->point.x = mouse_x;
    data->point.y = mouse_y;
    data->state = mouse_state;
}

void lvgl_input_init(void)
{
    LOG_CALL("lvgl_input_init\n");
    key_head = key_tail = 0;
    mouse_x = mouse_y = 0;
    mouse_state = LV_INDEV_STATE_RELEASED;

    key_indev = lv_indev_create();
    if(key_indev) {
        lv_indev_set_type(key_indev, LV_INDEV_TYPE_KEYPAD);
        lv_indev_set_read_cb(key_indev, keyboard_read);
        lv_indev_set_mode(key_indev, LV_INDEV_MODE_EVENT);
    }

    mouse_indev = lv_indev_create();
    if(mouse_indev) {
        lv_indev_set_type(mouse_indev, LV_INDEV_TYPE_POINTER);
        lv_indev_set_read_cb(mouse_indev, mouse_read);
        lv_indev_set_mode(mouse_indev, LV_INDEV_MODE_EVENT);
    }
}

void lvgl_input_push_key(unsigned int key, int pressed)
{
    int next = (key_tail + 1) % KEY_QUEUE_LEN;
    if(next == key_head) {
        return; /* drop */
    }
    key_queue[key_tail].key = key;
    key_queue[key_tail].state = pressed ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
    key_tail = next;
}

void lvgl_input_set_mouse_pos(int x, int y)
{
    mouse_x = (int16_t)x;
    mouse_y = (int16_t)y;
}

void lvgl_input_push_mouse_button(unsigned int button, int pressed)
{
    (void)button; /* only left button used */
    mouse_state = pressed ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
}


