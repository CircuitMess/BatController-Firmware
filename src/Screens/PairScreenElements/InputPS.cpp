
#include "InputPS.h"
#include "../../InputLVGL.h"
#include <lvgl.h>

#define LV_KB_BTN(width) width

static const char* const kb_map_lc[] = { "1#", "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", LV_SYMBOL_BACKSPACE, "\n",
										 "ABC", "a", "s", "d", "f", "g", "h", "j", "k", "l", "\n",
										 "_", "-", "z", "x", "c", "v", "b", "n", "m", ".", ",", ":", "\n",
										 LV_SYMBOL_CLOSE, LV_SYMBOL_LEFT, " ", LV_SYMBOL_RIGHT, LV_SYMBOL_OK, ""
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
		LV_KEYBOARD_CTRL_BTN_FLAGS | 2, LV_BTNMATRIX_CTRL_CHECKED | 2, 6, LV_BTNMATRIX_CTRL_CHECKED | 2, LV_KEYBOARD_CTRL_BTN_FLAGS | 2
};

static const char* const kb_map_uc[] = { "1#", "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", LV_SYMBOL_BACKSPACE, "\n",
										 "abc", "A", "S", "D", "F", "G", "H", "J", "K", "L", "\n",
										 "_", "-", "Z", "X", "C", "V", "B", "N", "M", ".", ",", ":", "\n",
										 LV_SYMBOL_CLOSE, LV_SYMBOL_LEFT, " ", LV_SYMBOL_RIGHT, LV_SYMBOL_OK, ""
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
		LV_KEYBOARD_CTRL_BTN_FLAGS | 2, LV_BTNMATRIX_CTRL_CHECKED | 2, 6, LV_BTNMATRIX_CTRL_CHECKED | 2, LV_KEYBOARD_CTRL_BTN_FLAGS | 2
};

static const char* const kb_map_spec[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", LV_SYMBOL_BACKSPACE, "\n",
										   "abc", "+", "-", "/", "*", "=", "%", "!", "?", "#", "<", ">", "\n",
										   "\\", "@", "$", "(", ")", "{", "}", "[", "]", ";", "\"", "'", "\n",
										   LV_SYMBOL_CLOSE, LV_SYMBOL_LEFT, " ", LV_SYMBOL_RIGHT, LV_SYMBOL_OK, ""
};

static const lv_btnmatrix_ctrl_t kb_ctrl_spec_map[] = {
		LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1),
		LV_KB_BTN(1), LV_KB_BTN(1), LV_BTNMATRIX_CTRL_CHECKED | 2,
		LV_KEYBOARD_CTRL_BTN_FLAGS | 2, LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(
				1),
		LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1),
		LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1),
		LV_KB_BTN(1), LV_KB_BTN(1), LV_KB_BTN(1),
		LV_KEYBOARD_CTRL_BTN_FLAGS | 2, LV_BTNMATRIX_CTRL_CHECKED | 2, 6, LV_BTNMATRIX_CTRL_CHECKED | 2, LV_KEYBOARD_CTRL_BTN_FLAGS | 2
};


InputPS::InputPS(lv_obj_t* obj, lv_group_t* inputGroup) : inputGroup(inputGroup){

	input = lv_obj_create(obj);
	lv_obj_set_style_bg_color(input, lv_color_black(), 0);
	lv_obj_set_style_bg_opa(input, LV_OPA_COVER, 0);
	lv_obj_set_size(input, 160, 128);
	lv_obj_set_pos(input, 0, 0);
	lv_obj_add_flag(input, LV_OBJ_FLAG_HIDDEN);
	lv_obj_set_scrollbar_mode(input, LV_SCROLLBAR_MODE_OFF);

	title = lv_label_create(input);
	lv_obj_set_size(title, 150, 15);
	lv_obj_set_pos(title, 10, 10);
	lv_obj_set_style_text_color(title, lv_palette_lighten(LV_PALETTE_GREY, 1), 0);
	lv_obj_set_style_text_font(title, &lv_font_unscii_8, 0);
	lv_label_set_text(title, "Enter WiFi name:");

	taNetwork = lv_textarea_create(input);
	lv_obj_align(taNetwork, LV_ALIGN_TOP_LEFT, 10, 30);
	lv_obj_set_style_text_font(taNetwork, &lv_font_unscii_8, 0);
	lv_textarea_set_one_line(taNetwork, true);
	lv_obj_set_style_text_color(taNetwork, lv_color_white(), 0);
	lv_textarea_set_max_length(taNetwork, 21);
	lv_obj_set_scrollbar_mode(taNetwork, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_size(taNetwork, 140, 20);
	lv_obj_add_state(taNetwork, LV_STATE_FOCUSED);

	lv_obj_add_event_cb(taNetwork, [](lv_event_t* e){
		auto input = static_cast<InputPS*>(lv_event_get_user_data(e));
		input->toPassword();
	}, LV_EVENT_READY, this);

	lv_obj_add_event_cb(taNetwork, [](lv_event_t* e){
		auto input = static_cast<InputPS*>(lv_event_get_user_data(e));

		if(!input->callbackBack) return;
		input->callbackBack();
	}, LV_EVENT_CANCEL, this);

	taPassword = lv_textarea_create(input);
	lv_obj_align(taPassword, LV_ALIGN_TOP_LEFT, 10, 30);
	lv_obj_set_style_text_font(taPassword, &lv_font_unscii_8, 0);
	lv_textarea_set_one_line(taPassword, true);
	lv_obj_set_style_text_color(taPassword, lv_color_white(), 0);
	lv_textarea_set_max_length(taPassword, 21);
	lv_obj_set_scrollbar_mode(taPassword, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_size(taPassword, 140, 20);
	lv_obj_add_flag(taPassword, LV_OBJ_FLAG_HIDDEN);

	lv_obj_add_event_cb(taPassword, [](lv_event_t* e){
		InputPS* input = static_cast<InputPS*>(lv_event_get_user_data(e));
		input->toNetwork();

		if(!input->callbackDone) return;
		input->callbackDone(lv_textarea_get_text(input->taNetwork), lv_textarea_get_text(input->taPassword));
	}, LV_EVENT_READY, this);


	lv_obj_add_event_cb(taPassword, [](lv_event_t* e){
		InputPS* input = static_cast<InputPS*>(lv_event_get_user_data(e));
		input->toNetwork();
	}, LV_EVENT_CANCEL, this);

	auto oldGroup = lv_group_get_default();
	lv_group_set_default(inputGroup);
	kb = lv_keyboard_create(input);
	lv_group_set_default(oldGroup);

	lv_keyboard_set_map(kb, LV_KEYBOARD_MODE_TEXT_UPPER, (const char**) kb_map_uc, kb_ctrl_uc_map);
	lv_keyboard_set_map(kb, LV_KEYBOARD_MODE_TEXT_LOWER, (const char**) kb_map_lc, kb_ctrl_lc_map);
	lv_keyboard_set_map(kb, LV_KEYBOARD_MODE_SPECIAL, (const char**) kb_map_spec, kb_ctrl_spec_map);
	lv_obj_set_scrollbar_mode(kb, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_style_text_font(kb, &lv_font_montserrat_10, 0);

	toNetwork();

	lv_group_focus_obj(kb);
}

InputPS::~InputPS(){
	lv_obj_remove_event_cb_with_user_data(taNetwork, nullptr, this);
	lv_obj_remove_event_cb_with_user_data(taPassword, nullptr, this);
	callbackDone = nullptr;
	callbackBack = nullptr;
}

void InputPS::start(){
	lv_obj_clear_flag(input, LV_OBJ_FLAG_HIDDEN);
	InputLVGL::enableVerticalNavigation(false);
	lv_obj_add_event_cb(kb, [](lv_event_t* e){
		if(lv_indev_get_key(lv_indev_get_act()) != LV_KEY_ESC) return;
		InputPS* input = static_cast<InputPS*>(lv_event_get_user_data(e));

		if(lv_keyboard_get_textarea(input->kb) == input->taNetwork){
			if(!input->callbackBack) return;
			input->callbackBack();
		}else{
			input->toNetwork();
		}

	}, LV_EVENT_CANCEL, this);

	lv_group_add_obj(inputGroup, kb);
	lv_group_focus_obj(kb);
}

void InputPS::stop(){
	lv_obj_add_flag(input, LV_OBJ_FLAG_HIDDEN);
	InputLVGL::enableVerticalNavigation(true);
	lv_obj_remove_event_cb_with_user_data(kb, nullptr, this);
	lv_group_remove_obj(kb);
}

void InputPS::setCallbackDone(std::function<void(std::string, std::string)> cb){
	callbackDone = cb;
}

void InputPS::setCallbackBack(std::function<void()> cb){
	callbackBack = cb;
}

void InputPS::toNetwork(){
	lv_obj_add_flag(taPassword, LV_OBJ_FLAG_HIDDEN);
	lv_obj_clear_flag(taNetwork, LV_OBJ_FLAG_HIDDEN);
	lv_obj_add_state(taNetwork, LV_STATE_FOCUSED);
	lv_keyboard_set_textarea(kb, taNetwork);
	lv_label_set_text(title, "Enter WiFi name:");
	lv_keyboard_set_mode(kb, LV_KEYBOARD_MODE_TEXT_UPPER);
	lv_btnmatrix_set_selected_btn((lv_obj_t*) &(((lv_keyboard_t*) kb)->btnm), 0);
}

void InputPS::toPassword(){
	lv_obj_add_flag(taNetwork, LV_OBJ_FLAG_HIDDEN);
	lv_obj_clear_flag(taPassword, LV_OBJ_FLAG_HIDDEN);
	lv_obj_add_state(taPassword, LV_STATE_FOCUSED);
	lv_keyboard_set_textarea(kb, taPassword);
	lv_label_set_text(title, "Enter password:");
	lv_keyboard_set_mode(kb, LV_KEYBOARD_MODE_TEXT_UPPER);
	lv_btnmatrix_set_selected_btn((lv_obj_t*) &(((lv_keyboard_t*) kb)->btnm), 0);
}

void InputPS::setNetwork(char* network){
	lv_textarea_set_text(taNetwork, network);
}

void InputPS::setPassword(char* password){
	lv_textarea_set_text(taPassword, password);
}
