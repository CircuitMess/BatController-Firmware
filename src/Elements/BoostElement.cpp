#include <Arduino.h>
#include "BoostElement.h"

BoostElement::BoostElement(lv_obj_t* parent) : LVObject(parent){

	lv_obj_set_size(obj, w, h);
	lv_obj_set_style_bg_opa(obj, LV_OPA_TRANSP, LV_STATE_DEFAULT);

	bgGIF = lv_gif_create(obj);
	lv_gif_set_src(bgGIF, activePath);
	lv_obj_set_pos(bgGIF, 0, 0);
	lv_obj_set_size(bgGIF, w, h);
	lv_obj_set_style_opa(bgGIF, LV_OPA_COVER, LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(bgGIF, LV_OPA_TRANSP, LV_STATE_DEFAULT);
	lv_obj_add_flag(bgGIF, LV_OBJ_FLAG_HIDDEN);

	bgIMG = lv_img_create(obj);
	lv_obj_set_pos(bgIMG, 0, 0);
	lv_obj_set_size(bgIMG, w, h);
	lv_obj_set_style_opa(bgIMG, LV_OPA_COVER, LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(bgIMG, LV_OPA_TRANSP, LV_STATE_DEFAULT);

	bar = lv_slider_create(obj);
	lv_obj_set_pos(bar, 9, 7);
	lv_obj_set_size(bar, 4, 92);

	lv_obj_set_style_bg_img_src(bar, fillSegmentPath, LV_PART_INDICATOR);
	lv_obj_set_style_bg_img_tiled(bar, true, LV_PART_INDICATOR);
	lv_obj_set_style_bg_opa(bar, LV_OPA_TRANSP, LV_STATE_DEFAULT);
	lv_obj_set_style_radius(bar, 3, LV_PART_MAIN);

	lv_obj_set_style_bg_opa(bar, LV_OPA_TRANSP, LV_PART_MAIN);
	lv_img_set_src(bgIMG, emptyPath);

	setActive(false);
	setLevel(0);
}

void BoostElement::setActive(bool active){
	if(active && !gifRunning){
		lv_obj_set_style_bg_opa(bar, LV_OPA_COVER, LV_PART_MAIN);
		lv_obj_add_flag(bgIMG, LV_OBJ_FLAG_HIDDEN);
		lv_obj_clear_flag(bgGIF, LV_OBJ_FLAG_HIDDEN);

		lv_gif_start(bgGIF);
		lv_gif_set_loop(bgGIF, LV_GIF_LOOP_ON);
		gifRunning = true;

	}else if(!active && gifRunning){
		lv_obj_set_style_bg_opa(bar, LV_OPA_TRANSP, LV_PART_MAIN);
		lv_obj_clear_flag(bgIMG, LV_OBJ_FLAG_HIDDEN);
		lv_obj_add_flag(bgGIF, LV_OBJ_FLAG_HIDDEN);
		lv_gif_stop(bgGIF);

		lv_img_set_src(bgIMG, emptyPath);
		gifRunning = false;
	}
}

void BoostElement::setLevel(uint8_t level){
	level = min((uint8_t)100, level);
	lv_bar_set_value(bar, level, LV_ANIM_OFF);
	if(gifRunning) return;

	if(level == 100){
		lv_img_set_src(bgIMG, fullPath);
	}else{
		lv_img_set_src(bgIMG, emptyPath);
	}
}
