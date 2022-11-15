#include "OverrideElement.h"

OverrideElement::OverrideElement(lv_obj_t* parent) : LVObject(parent){
	lv_obj_set_size(obj, w, h);
	lv_obj_set_style_bg_img_src(obj, "S:/DriveScreen/ModalBg.bin", LV_STATE_DEFAULT);
	lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_row(obj, 10, LV_STATE_DEFAULT);

	text = lv_label_create(obj);
	lv_obj_align(text, LV_ALIGN_CENTER, 0, 0);
	lv_label_set_text(text, "MANUAL\nOVERRIDE");
	lv_obj_set_style_text_align(text, LV_TEXT_ALIGN_CENTER, 0);
	lv_obj_set_style_text_font(text, &lv_font_montserrat_8, 0);

	slider = lv_slider_create(obj);
	lv_slider_set_range(slider, 0, sliderRange);
	lv_slider_set_value(slider, map(0, 0, 100, 0, sliderRange), LV_ANIM_OFF);
	lv_obj_set_size(slider, 65, 5);
	lv_obj_set_style_pad_left(slider, 1, LV_PART_MAIN);
	lv_obj_set_style_pad_right(slider, 2, LV_PART_MAIN);

	lv_obj_set_style_bg_img_src(slider, "S:/DriveScreen/SpeedBar.bin", LV_PART_MAIN);
	lv_obj_set_style_bg_img_src(slider, "S:/DriveScreen/SpeedBarFill.bin", LV_PART_INDICATOR);
	lv_obj_set_style_bg_img_tiled(slider, true, LV_PART_INDICATOR);

	lv_obj_update_layout(parent);
}

OverrideElement::~OverrideElement(){}

void OverrideElement::fill(uint8_t percent){
	lv_slider_set_value(slider, map(percent, 0, 100, 0, sliderRange), LV_ANIM_ON);
}