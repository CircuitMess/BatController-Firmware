
#include "InputPS.h"
#include "../../InputLVGL.h"
#include <lvgl.h>

#define LV_KB_BTN(width) LV_BTNMATRIX_CTRL_POPOVER | width

static const char *const kb_map_lc[] = {"1#", "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", LV_SYMBOL_BACKSPACE,"\n",
                                        "ABC", "a", "s", "d", "f", "g", "h", "j", "k", "l", "\n",
                                        "_", "-", "z", "x", "c", "v", "b", "n", "m", ".", ",", ":", "\n",
                                        LV_SYMBOL_LEFT, " ", LV_SYMBOL_RIGHT, LV_SYMBOL_OK, ""
};

static const lv_btnmatrix_ctrl_t kb_ctrl_lc_map[] = {
        LV_KEYBOARD_CTRL_BTN_FLAGS | 5, LV_KB_BTN(4), LV_KB_BTN(4), LV_KB_BTN(4), LV_KB_BTN(4), LV_KB_BTN(4),
        LV_KB_BTN(4), LV_KB_BTN(4), LV_KB_BTN(4), LV_KB_BTN(4), LV_KB_BTN(4), LV_BTNMATRIX_CTRL_CHECKED | 7,
        LV_KEYBOARD_CTRL_BTN_FLAGS | 6, LV_KB_BTN(3), LV_KB_BTN(3), LV_KB_BTN(3), LV_KB_BTN(3), LV_KB_BTN(3), LV_KB_BTN(
                                                                                                                      3),
        LV_KB_BTN(3), LV_KB_BTN(3), LV_KB_BTN(3),
        LV_BTNMATRIX_CTRL_CHECKED | LV_KB_BTN(1), LV_BTNMATRIX_CTRL_CHECKED | LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1),
        LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1), LV_BTNMATRIX_CTRL_CHECKED | LV_KB_BTN(1),
        LV_BTNMATRIX_CTRL_CHECKED | LV_KB_BTN(1), LV_BTNMATRIX_CTRL_CHECKED | LV_KB_BTN(1),
        LV_BTNMATRIX_CTRL_CHECKED | 2, 6, LV_BTNMATRIX_CTRL_CHECKED | 2, LV_KEYBOARD_CTRL_BTN_FLAGS | 2
};

static const char * const kb_map_uc[] = {"1#", "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", LV_SYMBOL_BACKSPACE,"\n",
                                 "abc", "A", "S", "D", "F", "G", "H", "J", "K", "L", "\n",
                                 "_", "-", "Z", "X", "C", "V", "B", "N", "M", ".", ",", ":", "\n",
                                 LV_SYMBOL_LEFT, " ", LV_SYMBOL_RIGHT, LV_SYMBOL_OK, ""
};

static const lv_btnmatrix_ctrl_t kb_ctrl_uc_map[] = {
        LV_KEYBOARD_CTRL_BTN_FLAGS | 5, LV_KB_BTN(4), LV_KB_BTN(4), LV_KB_BTN(4), LV_KB_BTN(4), LV_KB_BTN(4),
        LV_KB_BTN(4), LV_KB_BTN(4), LV_KB_BTN(4), LV_KB_BTN(4), LV_KB_BTN(4), LV_BTNMATRIX_CTRL_CHECKED | 7,
        LV_KEYBOARD_CTRL_BTN_FLAGS | 6, LV_KB_BTN(3), LV_KB_BTN(3), LV_KB_BTN(3), LV_KB_BTN(3), LV_KB_BTN(3), LV_KB_BTN(
                                                                                                                      3),
        LV_KB_BTN(3), LV_KB_BTN(3), LV_KB_BTN(3),
        LV_BTNMATRIX_CTRL_CHECKED | LV_KB_BTN(1), LV_BTNMATRIX_CTRL_CHECKED | LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1),
        LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1), LV_BTNMATRIX_CTRL_CHECKED | LV_KB_BTN(1),
        LV_BTNMATRIX_CTRL_CHECKED | LV_KB_BTN(1), LV_BTNMATRIX_CTRL_CHECKED | LV_KB_BTN(1),
        LV_BTNMATRIX_CTRL_CHECKED | 2, 6, LV_BTNMATRIX_CTRL_CHECKED | 2, LV_KEYBOARD_CTRL_BTN_FLAGS | 2
};

static const char *const kb_map_spec[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0", LV_SYMBOL_BACKSPACE, "\n",
                                          "abc", "+", "-", "/", "*", "=", "%", "!", "?", "#", "<", ">", "\n",
                                          "\\", "@", "$", "(", ")", "{", "}", "[", "]", ";", "\"", "'", "\n",
                                          LV_SYMBOL_LEFT, " ", LV_SYMBOL_RIGHT, LV_SYMBOL_OK, ""
};

static const lv_btnmatrix_ctrl_t kb_ctrl_spec_map[] = {
        LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1),
        LV_KB_BTN(1), LV_KB_BTN(1), LV_BTNMATRIX_CTRL_CHECKED | 2,
        LV_KEYBOARD_CTRL_BTN_FLAGS | 2, LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(
                                                                                                                      1),
        LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1),
        LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1),
        LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1),
        LV_BTNMATRIX_CTRL_CHECKED | 2, 6, LV_BTNMATRIX_CTRL_CHECKED | 2, LV_KEYBOARD_CTRL_BTN_FLAGS | 2
};



InputPS::InputPS(lv_obj_t *obj, lv_group_t *inputGroup) {

    input = lv_obj_create(obj);
    lv_obj_set_style_bg_color(input, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(input, LV_OPA_COVER, 0);
    lv_obj_set_size(input, 160, 128);
    lv_obj_set_pos(input, 0, 0);
    lv_obj_add_flag(input, LV_OBJ_FLAG_HIDDEN);

    taNetwork = lv_textarea_create(input);
    lv_obj_align(taNetwork, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_textarea_set_placeholder_text(taNetwork, "Network");
    lv_textarea_set_one_line(taNetwork, true);
    lv_obj_set_size(taNetwork, 120, 60);
    lv_obj_add_state(taNetwork, LV_STATE_FOCUSED);
    lv_obj_add_event_cb(taNetwork, [](lv_event_t *e) {
        lv_event_code_t code = lv_event_get_code(e);
        InputPS *input = static_cast<InputPS *>(lv_event_get_user_data(e));

        if (code == LV_EVENT_READY) {
            lv_obj_add_flag(input->taNetwork, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(input->taPassword, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_state(input->taPassword, LV_STATE_FOCUSED);
            lv_keyboard_set_textarea(input->kb, input->taPassword);
        }

    }, LV_EVENT_ALL, this);

    taPassword = lv_textarea_create(input);
    lv_obj_align(taPassword, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_add_flag(taPassword, LV_OBJ_FLAG_HIDDEN);
    lv_textarea_set_password_mode(taPassword, true);
    lv_textarea_set_placeholder_text(taPassword, "Password");
    lv_textarea_set_one_line(taPassword, true);
    lv_obj_set_size(taPassword, 120, 60);

    auto oldGroup = lv_group_get_default();
    lv_group_set_default(inputGroup);
    kb = lv_keyboard_create(input);
    lv_group_set_default(oldGroup);

    lv_keyboard_set_map(kb, LV_KEYBOARD_MODE_TEXT_UPPER, (const char * *)kb_map_uc, kb_ctrl_uc_map);
    lv_keyboard_set_map(kb, LV_KEYBOARD_MODE_TEXT_LOWER, (const char * *)kb_map_lc, kb_ctrl_lc_map);
    lv_keyboard_set_map(kb, LV_KEYBOARD_MODE_SPECIAL, (const char * *)kb_map_spec, kb_ctrl_spec_map);

    lv_keyboard_set_textarea(kb, taNetwork);

    lv_group_focus_obj(kb);
}

InputPS::~InputPS() {

}

void InputPS::start() {
    lv_obj_clear_flag(input, LV_OBJ_FLAG_HIDDEN);
    InputLVGL::enableVerticalNavigation(false);

}

void InputPS::stop() {
    lv_obj_add_flag(input, LV_OBJ_FLAG_HIDDEN);
    InputLVGL::enableVerticalNavigation(true);
}