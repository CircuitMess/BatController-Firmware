#include "HueModal.h"
#include <BatController.h>

HueModal::HueModal(LVScreen* parent, std::function<void(uint8_t)> hueCB, uint8_t currentHue) : LVModal(parent), hueCB(hueCB){

	lv_obj_set_size(obj, w, h);
	lv_obj_set_style_bg_img_src(obj, "S:/DriveScreen/ModalBg.bin", LV_STATE_DEFAULT);
	lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_row(obj, 20, LV_STATE_DEFAULT);
//	lv_obj_set_style_pad_all(obj, 7, LV_STATE_DEFAULT);

	text = lv_img_create(obj);
	lv_img_set_src(text, "S:/DriveScreen/BallHueText.bin");

	slider = lv_slider_create(obj);
	lv_slider_set_range(slider, 0, sliderRange);
	lv_slider_set_value(slider, map(currentHue, 0, 180, 0, sliderRange), LV_ANIM_OFF);
	lv_obj_set_size(slider, 65, 5);
	lv_obj_set_style_pad_left(slider, 1, LV_PART_MAIN);
	lv_obj_set_style_pad_right(slider, 1, LV_PART_MAIN);
	lv_group_add_obj(inputGroup, slider);
	//knob horizontal padding - offset from slider left/right, knob vertical padding - offset from slider top/bottom
	lv_obj_set_style_pad_top(slider, 14, LV_PART_KNOB);
	lv_obj_set_style_pad_hor(slider, 4, LV_PART_KNOB);
	lv_obj_set_style_bg_img_src(slider, "S:/DriveScreen/BallHueBar.bin", LV_PART_MAIN);
	lv_obj_set_style_bg_img_src(slider, "S:/DriveScreen/BallHuePicker.bin", LV_PART_KNOB);


	lv_obj_update_layout(parent->getLvObj());

	knobCircle = lv_obj_create(obj);
	lv_obj_add_flag(knobCircle, LV_OBJ_FLAG_IGNORE_LAYOUT);
	lv_obj_add_flag(knobCircle, LV_OBJ_FLAG_FLOATING);
	lv_obj_set_size(knobCircle, 9, 9);
	lv_obj_set_style_radius(knobCircle, 3, LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(knobCircle, LV_OPA_COVER, LV_STATE_DEFAULT);
	auto color = lv_color_hsv_to_rgb(currentHue * 2, 100, 100);
	lv_obj_set_style_bg_color(knobCircle, color, LV_STATE_DEFAULT);
	auto x = map(lv_slider_get_value(slider), 0, sliderRange, 0, 63) + lv_obj_get_x(slider) - lv_obj_get_style_pad_left(slider, LV_PART_KNOB) +
			 lv_obj_get_style_pad_left(slider, LV_PART_MAIN);
	auto y = lv_obj_get_y(slider) - lv_obj_get_style_pad_top(slider, LV_PART_KNOB) + 1;
	lv_obj_set_pos(knobCircle, x, y);

	lv_obj_add_event_cb(slider, [](lv_event_t* e){
		auto drawParams = (lv_obj_draw_part_dsc_t*) e->param;
		if(drawParams->part != LV_PART_KNOB) return;
		auto circle = (lv_obj_t*) e->user_data;
		auto slider = e->target;
		auto color = lv_color_hsv_to_rgb(map(lv_slider_get_value(slider), 0, sliderRange, 0, 360), 100, 100);
		auto x = map(lv_slider_get_value(slider), 0, sliderRange, 0, 63) + lv_obj_get_x(slider) - lv_obj_get_style_pad_left(slider, LV_PART_KNOB) +
				 lv_obj_get_style_pad_left(slider, LV_PART_MAIN);
		lv_obj_set_x(circle, x);
		lv_obj_set_style_bg_color(circle, color, LV_STATE_DEFAULT);
	}, LV_EVENT_DRAW_PART_END, knobCircle);

	lv_obj_add_event_cb(slider, [](lv_event_t* e){
		auto modal = (HueModal*) e->user_data;
		lv_timer_reset(modal->timeout);
		if(!modal->hueCB) return;
		modal->hueCB(map(lv_slider_get_value(modal->slider), 0, sliderRange, 0, 180));
	}, LV_EVENT_VALUE_CHANGED, this);


	lv_obj_add_event_cb(slider, [](lv_event_t* e){
		auto key = lv_event_get_key(e);
		auto modal = ((HueModal*) e->user_data);
		if(modal->isActive() && (key == LV_KEY_ESC || key == LV_KEY_ENTER)){
			modal->stop();
		}else{
			lv_timer_reset(modal->timeout);
		}
	}, LV_EVENT_KEY, this);
}

HueModal::~HueModal(){
	stop();
}

void HueModal::onStart(){
	lv_group_focus_obj(slider);

	timeout = lv_timer_create([](lv_timer_t* timer){
		auto modal = ((HueModal*) timer->user_data);
		if(modal->isActive()) modal->stop();
	}, timeoutValue, this);
}

void HueModal::onStop(){
	if(timeout){
		lv_timer_del(timeout);
		timeout = nullptr;
	}

	if(!hueCB) return;
	hueCB(map(lv_slider_get_value(slider), 0, sliderRange, 0, 180));
}