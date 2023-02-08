#include "MessageModal.h"

MessageModal::MessageModal(LVScreen* parent, const char* message) : LVModal(parent){
	lv_obj_set_size(obj, 79, 48);
	lv_obj_set_style_bg_img_src(obj, "S:/DriveScreen/ModalBg.bin", 0);
	lv_obj_set_style_bg_img_opa(obj, LV_OPA_COVER, 0);

	lv_obj_t* txt = lv_label_create(obj);
	lv_obj_set_width(txt, 73);
	lv_obj_center(txt);
	lv_label_set_text(txt, message);
	lv_label_set_long_mode(txt, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_color(txt, lv_color_white(), 0);
	lv_obj_set_style_text_font(txt, &lv_font_unscii_8, 0);
	lv_obj_set_style_text_align(txt, LV_TEXT_ALIGN_CENTER, 0);

	lv_obj_add_event_cb(txt, [](lv_event_t* e){
		auto modal = static_cast<MessageModal*>(lv_event_get_user_data(e));
		modal->stop();
		delete modal;
	}, LV_EVENT_CLICKED, this);

	lv_group_add_obj(inputGroup, txt);
	lv_group_focus_obj(txt);
}