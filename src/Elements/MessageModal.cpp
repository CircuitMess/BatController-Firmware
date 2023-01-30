#include "MessageModal.h"

MessageModal::MessageModal(LVScreen* parent, const char* message) : LVModal(parent){
	lv_obj_t* panel = lv_img_create(obj);
	lv_obj_set_style_bg_img_src(panel, "S:/DriveScreen/ModalBg.bin", LV_STATE_DEFAULT);
	lv_obj_set_size(panel, 79, 48);
	lv_obj_center(panel);
	lv_obj_add_event_cb(panel, [](lv_event_t* e){
		auto tmp = static_cast<MessageModal*>(lv_event_get_user_data(e));
		tmp->stop();
	}, LV_EVENT_KEY, this);

	lv_obj_t* txt = lv_label_create(panel);
	lv_obj_set_style_text_color(txt, lv_color_white(), 0);
	lv_obj_set_style_text_font(txt, &lv_font_unscii_8, 0);
	lv_label_set_text(txt, message);
	lv_label_set_long_mode(txt, LV_LABEL_LONG_WRAP);
	lv_obj_center(txt);

	lv_group_add_obj(inputGroup, panel);
	lv_group_focus_obj(panel);
}

MessageModal::~MessageModal(){

}

void MessageModal::onStart(){
	LVModal::onStart();
}

void MessageModal::onStop(){
	LVModal::onStop();
}
