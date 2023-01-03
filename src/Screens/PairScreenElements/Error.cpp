#include <lvgl.h>
#include "Error.h"

Error::Error(lv_obj_t* obj, lv_group_t* inputGroup) : inputGroup(inputGroup){
	error = lv_obj_create(obj);
	lv_obj_set_style_bg_color(error, lv_color_black(), 0);
	lv_obj_set_style_bg_opa(error, LV_OPA_COVER, 0);
	lv_obj_set_size(error, 160, 128);
	lv_obj_set_pos(error, 0, 0);
	lv_obj_add_flag(error, LV_OBJ_FLAG_HIDDEN);

	img = lv_img_create(error);
	lv_img_set_src(img, "S:/Pairing/WiFiOff.bin");
	lv_obj_set_pos(img, 59, 36);

	text = lv_label_create(error);
	lv_obj_set_size(text, 120, 40);
	lv_obj_set_pos(text, 20, 80);
	lv_obj_set_style_text_color(text, lv_color_white(), 0);
	lv_obj_set_style_text_font(text, &lv_font_unscii_8, 0);
}

Error::~Error(){
	callback = nullptr;
}

void Error::start(std::string errorMessage){
	lv_label_set_text(text, errorMessage.c_str());
	lv_obj_clear_flag(error, LV_OBJ_FLAG_HIDDEN);

	lv_obj_add_event_cb(error, [](lv_event_t* e){
		if(lv_indev_get_key(lv_indev_get_act()) != LV_KEY_ENTER) return;
		Error* error = static_cast<Error*>(lv_event_get_user_data(e));

		if(!error->callback) return;
		error->callback();
	}, LV_EVENT_RELEASED, this);
	lv_group_add_obj(inputGroup, error);
	lv_group_focus_obj(error);
}

void Error::stop(){
	lv_obj_add_flag(error, LV_OBJ_FLAG_HIDDEN);
	lv_obj_remove_event_cb_with_user_data(error, nullptr, this);
	lv_group_remove_obj(error);
}

void Error::setCallback(std::function<void()> cb){
	callback = cb;
}
