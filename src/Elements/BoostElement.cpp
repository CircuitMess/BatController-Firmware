#include "BoostElement.h"

BoostElement::BoostElement(lv_obj_t* parent) : LVObject(parent){

	lv_obj_set_size(obj, w, h);

	bgGIF = lv_gif_create(obj);
	lv_obj_set_pos(bgGIF, 0, 0);
	lv_obj_set_size(bgGIF, w, h);
	lv_obj_set_style_opa(bgGIF, LV_OPA_COVER, LV_STATE_DEFAULT);

	bar = lv_slider_create(obj);
	lv_obj_set_pos(bar, 9, 7);
	lv_obj_set_size(bar, 4, 91);

	lv_obj_set_style_bg_img_src(bar, fillSegmentPath, LV_PART_INDICATOR);
	lv_obj_set_style_bg_img_tiled(bar, true, LV_PART_INDICATOR);

	setActive(false);
	setLevel(0);
}

void BoostElement::setActive(bool active){
	if(active){
		lv_obj_set_style_bg_opa(bar, LV_OPA_COVER, LV_PART_INDICATOR);
		lv_gif_set_src(bgGIF, activePath);
		lv_gif_set_loop(bgGIF, LV_GIF_LOOP_ON);

	}else{
		lv_obj_set_style_bg_opa(bar, LV_OPA_TRANSP, LV_PART_INDICATOR);
		lv_img_set_src(bgGIF, emptyPath);
	}
}

void BoostElement::setLevel(uint8_t level){
	lv_bar_set_value(bar, level, LV_ANIM_OFF);
}
