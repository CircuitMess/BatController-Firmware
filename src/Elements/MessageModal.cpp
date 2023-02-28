#include "MessageModal.h"
#include <Loop/LoopManager.h>
#include <Input/Input.h>

MessageModal::MessageModal(LVScreen* parent, const char* message, uint32_t timeout) : LVModal(parent){
	lv_obj_set_size(obj, 79, 48);
	lv_obj_set_style_bg_img_src(obj, "S:/DriveScreen/ModalBg.bin", 0);
	lv_obj_set_style_bg_img_opa(obj, LV_OPA_COVER, 0);

	lv_obj_t* txt = lv_label_create(obj);
	lv_obj_set_width(txt, 73);
	lv_obj_center(txt);
	lv_label_set_text(txt, message);
	lv_label_set_long_mode(txt, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_color(txt, lv_color_white(), 0);
	lv_obj_set_style_text_font(txt, &lv_font_montserrat_10, 0);
	lv_obj_set_style_text_align(txt, LV_TEXT_ALIGN_CENTER, 0);

	if(timeout != UINT32_MAX){
		timer = lv_timer_create([](lv_timer_t* t){
			auto modal = static_cast<MessageModal*>(t->user_data);
			modal->dismiss();
		}, timeout, this);
		lv_timer_set_repeat_count(timer, 1);
		lv_timer_pause(timer);
	}
}

void MessageModal::onStart(){
	if(timer){
		lv_timer_reset(timer);
		lv_timer_resume(timer);
	}
	startMillis = millis();
	Input::getInstance()->addListener(this);
}

void MessageModal::onStop(){
	if(timer){
		lv_timer_pause(timer);
	}

	Input::getInstance()->removeListener(this);
}

void MessageModal::buttonReleased(uint i){
	if(millis() - startMillis < nonDismissMillis) return;
	LoopManager::defer([this](uint32_t t){
		dismiss();
	});
}

void MessageModal::setDismissCallback(std::function<void()> dismissCallback){
	MessageModal::dismissCallback = std::move(dismissCallback);
}

void MessageModal::dismiss(){
	std::function<void()> callback = dismissCallback;

	stop();
	delete this;

	if(callback){
		callback();
	}
}
