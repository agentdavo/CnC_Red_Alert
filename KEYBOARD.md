# Keyboard Input

`CODE/key.h` defines the keyboard queue used by the engine. It sets bit masks for shift, control and other modifiers, then exposes the `WWKeyboardClass` interface.

## CODE/key.h
- The `WWKey_Type` enum lists modifier bits like `WWKEY_SHIFT_BIT` and `WWKEY_RLS_BIT`【F:CODE/key.h†L53-L61】.
- `WWKeyboardClass` stores a 256 entry state array and a circular buffer to hold pending key presses【F:CODE/key.h†L63-L109】.
- Two enums translate between ASCII keys and virtual key codes. `KeyNumType` begins at zero and maps codes such as `KN_F1` and `KN_SPACE` to `VK_` values【F:CODE/key.h†L520-L644】.

## CODE/KEYBOARD.CPP
- `Put_Key_Message` assembles the modifier bits and pushes a key into the queue before passing it to `Put`【F:CODE/KEYBOARD.CPP†L176-L219】.
- `To_ASCII` converts a queued key to an ASCII character using Windows `ToAscii` and `MapVirtualKey` functions【F:CODE/KEYBOARD.CPP†L226-L297】.

## Using LVGL `lv_keyboard`
The LVGL demo shows how to spawn a keyboard widget for text areas. A call to `lv_keyboard_create` returns an object that can be toggled by user input:
```c
lv_obj_t * kb = lv_keyboard_create(lv_screen_active());
```
Event callbacks attach the keyboard to a focused textarea and reveal it for encoder input【F:src/lvgl/demos/keypad_encoder/lv_demo_keypad_encoder.c†L150-L209】.

When integrating with the game, `lv_keyboard_set_textarea` should be called from the event handler whenever LVGL reports a `LV_EVENT_CLICKED` from an encoder or keypad. The underlying key events will then flow through LVGL's input devices and can be read back via `lv_indev_get_type`.
