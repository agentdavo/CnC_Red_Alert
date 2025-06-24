# Mouse Input

`CODE/mouse.h` describes the animated mouse cursor handler. `MouseClass` inherits from `ScrollClass` and manages frame timing, shape switching and file I/O.

## CODE/mouse.h
- The handler exposes methods like `Override_Mouse_Shape`, `Revert_Mouse_Shape` and `AI` to update the pointer each tick【F:CODE/mouse.h†L52-L70】.
- `MouseStruct` holds frame information and hotspot offsets for each cursor shape【F:CODE/mouse.h†L84-L101】.
- `IsSmall` tracks whether the small cursor variant is active【F:CODE/mouse.h†L100-L106】.

## CODE/MOUSE.CPP
- `Set_Default_Mouse` sets the current and normal cursor types before delegating to `Override_Mouse_Shape`【F:CODE/MOUSE.CPP†L92-L105】.
- `Override_Mouse_Shape` changes the cursor sprite and records whether the small version is displayed【F:CODE/MOUSE.CPP†L184-L229】.
- The `AI` method advances animated cursors based on the timer and forwards input to `ScrollClass::AI`【F:CODE/MOUSE.CPP†L236-L271】.

## Using LVGL `lv_indev`
LVGL groups input devices behind the `lv_indev` API. The keypad/encoder demo queries each device with `lv_indev_get_next` and assigns them to a group for focus handling:
```c
lv_indev_t * indev = NULL;
for(;;) {
    indev = lv_indev_get_next(indev);
    if(!indev) break;
    lv_indev_type_t indev_type = lv_indev_get_type(indev);
    if(indev_type == LV_INDEV_TYPE_KEYPAD || indev_type == LV_INDEV_TYPE_ENCODER)
        lv_indev_set_group(indev, g);
}
```
The active device is retrieved with `lv_indev_active` inside the event callback to attach the onscreen keyboard【F:src/lvgl/demos/keypad_encoder/lv_demo_keypad_encoder.c†L48-L66】【F:src/lvgl/demos/keypad_encoder/lv_demo_keypad_encoder.c†L192-L209】.
