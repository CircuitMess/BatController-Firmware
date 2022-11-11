#include "SpeedModal.h"
#include <BatController.h>

SpeedModal::SpeedModal(LVScreen* parent, std::function<void(uint8_t)> speedCB, uint8_t currentSpeed) : LVModal(parent), speedCB(speedCB){

	lv_obj_set_size(obj, w, h);
	lv_obj_set_style_bg_img_src(obj, "S:/DriveScreen/ModalBg.bin", LV_STATE_DEFAULT);
	lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_row(obj, 20, LV_STATE_DEFAULT);
//	lv_obj_set_style_pad_all(obj, 7, LV_STATE_DEFAULT);

	text = lv_img_create(obj);
	lv_img_set_src(text, "S:/DriveScreen/SpeedText.bin");

	slider = lv_slider_create(obj);
	lv_slider_set_range(slider, 0, sliderRange);
	lv_slider_set_value(slider, map(currentSpeed, 0, 100, 0, sliderRange), LV_ANIM_OFF);
	lv_obj_set_size(slider, 65, 5);
	lv_obj_set_style_pad_left(slider, 1, LV_PART_MAIN);
	lv_obj_set_style_pad_right(slider, 2, LV_PART_MAIN);
	lv_group_add_obj(inputGroup, slider);

	lv_obj_set_style_bg_img_src(slider, "S:/DriveScreen/SpeedBar.bin", LV_PART_MAIN);
	lv_obj_set_style_bg_img_src(slider, "S:/DriveScreen/SpeedBarFill.bin", LV_PART_INDICATOR);
	lv_obj_set_style_bg_img_tiled(slider, true, LV_PART_INDICATOR);


	lv_obj_update_layout(parent->getLvObj());

	lv_obj_add_event_cb(slider, [](lv_event_t* e){
		auto modal = (SpeedModal*) e->user_data;
		lv_timer_reset(modal->timeout);
		if(!modal->speedCB) return;
		modal->speedCB(map(lv_slider_get_value(modal->slider), 0, sliderRange, 0, 100));
	}, LV_EVENT_VALUE_CHANGED, this);


	lv_obj_add_event_cb(slider, [](lv_event_t* e){
		auto key = lv_event_get_key(e);
		auto modal = ((SpeedModal*) e->user_data);
		if(modal->isActive() && (key == LV_KEY_ESC || key == LV_KEY_ENTER)){
			modal->stop();
		}else{
			lv_timer_reset(modal->timeout);
		}
	}, LV_EVENT_KEY, this);
}


void SpeedModal::onStart(){
	lv_group_focus_obj(slider);

	timeout = lv_timer_create([](lv_timer_t* timer){
		auto modal = ((SpeedModal*) timer->user_data);
		if(modal->isActive()) modal->stop();
	}, timeoutValue, this);
}

void SpeedModal::onStop(){
	lv_timer_del(timeout);
	if(!speedCB) return;
	speedCB(map(lv_slider_get_value(slider), 0, sliderRange, 0, 100));
}


