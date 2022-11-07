#include "HueModal.h"
#include <BatController.h>

HueModal::HueModal(LVScreen* parent, std::function<void(uint8_t)> hueCB, uint8_t currentHue) : LVModal(parent){

	//hold for faster scroll input registration
	setButtonHoldAndRepeatTime(BTN_LEFT, holdRepeatTime);
	setButtonHoldAndRepeatTime(BTN_RIGHT, holdRepeatTime);
	setButtonHoldTime(BTN_LEFT, holdTime);
	setButtonHoldTime(BTN_RIGHT, holdTime);

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
	lv_slider_set_range(slider, 0, 64);
	lv_slider_set_value(slider, map(currentHue, 0, 180, 0, 64), LV_ANIM_OFF);
	lv_obj_set_size(slider, 64, 5);
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
	auto x = lv_slider_get_value(slider) + lv_obj_get_x(slider) - lv_obj_get_style_pad_left(slider, LV_PART_KNOB);
	auto y = lv_obj_get_y(slider) - lv_obj_get_style_pad_top(slider, LV_PART_KNOB) + 1;
	lv_obj_set_pos(knobCircle, x, y);

	lv_obj_add_event_cb(slider, knobCircleEventDraw, LV_EVENT_DRAW_PART_END, knobCircle);

}

void HueModal::knobCircleEventValue(lv_event_t* e){
	auto circle = (lv_obj_t*) e->user_data;
	auto slider = e->target;
	auto color = lv_color_hsv_to_rgb(map(lv_slider_get_value(slider), 0, 64, 0, 360), 100, 100);
	auto x = lv_slider_get_value(slider) + lv_obj_get_x(slider) - lv_obj_get_style_pad_left(slider, LV_PART_KNOB);
	lv_obj_set_x(circle, x);
	lv_obj_set_style_bg_color(circle, color, LV_STATE_DEFAULT);
	lv_obj_invalidate(slider);
	lv_obj_invalidate(circle);
}

void HueModal::knobCircleEventDraw(lv_event_t* e){
	auto circle = (lv_obj_t*) e->user_data;
	auto slider = e->target;
	auto color = lv_color_hsv_to_rgb(map(lv_slider_get_value(slider), 0, 64, 0, 360), 100, 100);
	auto x = lv_slider_get_value(slider) + lv_obj_get_x(slider) - lv_obj_get_style_pad_left(slider, LV_PART_KNOB);
	lv_obj_set_x(circle, x);
	lv_obj_set_style_bg_color(circle, color, LV_STATE_DEFAULT);
	lv_obj_invalidate(slider);
	lv_obj_invalidate(circle);
}

void HueModal::onStart(){
	Input::getInstance()->addListener(this);
	lv_group_focus_obj(slider);
}

void HueModal::onStop(){
	Input::getInstance()->removeListener(this);
	hueCB(map(lv_slider_get_value(slider), 0, 64, 0, 180));
}


void HueModal::buttonHeldRepeat(uint i, uint repeatCount){
	if(!heldThresh) return;

	if(i == BTN_LEFT && lv_slider_get_value(slider) > lv_slider_get_min_value(slider)){
		lv_slider_set_value(slider, lv_slider_get_value(slider) - increment, LV_ANIM_OFF);
	}else if(i == BTN_RIGHT && lv_slider_get_value(slider) < lv_slider_get_max_value(slider)){
		lv_slider_set_value(slider, lv_slider_get_value(slider) + increment, LV_ANIM_OFF);
	}

	lv_obj_invalidate(slider);
	lv_event_send(slider, LV_EVENT_VALUE_CHANGED, nullptr);

	incrementStepUsage++;
	if(incrementStepUsage >= incrementStep){
		increment *= 2;
		incrementStepUsage = 0;
	}
}

void HueModal::buttonReleased(uint i){
	if(!heldThresh) return;
	if(i == BTN_LEFT || i == BTN_RIGHT){
		heldThresh = false;
		increment = 1;
		incrementStepUsage = 0;
		Serial.println("event removed");
		lv_obj_remove_event_cb(slider, knobCircleEventValue);
		lv_obj_add_event_cb(slider, knobCircleEventDraw, LV_EVENT_DRAW_PART_END, knobCircle);
	}
}

void HueModal::buttonHeld(uint i){
	if(i == BTN_LEFT || i == BTN_RIGHT){
		heldThresh = true;
		Serial.println("event added");
		lv_obj_add_event_cb(slider, knobCircleEventValue, LV_EVENT_VALUE_CHANGED, knobCircle);
		lv_obj_remove_event_cb(slider, knobCircleEventDraw);

	}
}