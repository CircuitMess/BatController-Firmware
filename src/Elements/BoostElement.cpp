#include <Arduino.h>
#include <Loop/LoopManager.h>
#include "BoostElement.h"

BoostElement::BoostElement(lv_obj_t* parent) : LVObject(parent){

	lv_obj_set_size(obj, w, h);
	lv_obj_set_style_bg_opa(obj, LV_OPA_TRANSP, LV_STATE_DEFAULT);

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

BoostElement::~BoostElement(){
	LoopManager::removeListener(this);
}

void BoostElement::setActive(bool active){
	if(active && !gifRunning){
		gifRunning = true;
		nextFrame();
		LoopManager::addListener(this);
	}else if(!active && gifRunning){
		gifRunning = false;
		lv_img_set_src(bgIMG, emptyPath);
		LoopManager::removeListener(this);
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

bool BoostElement::isActive() const{
	return gifRunning;
}

void BoostElement::loop(uint micros){
	if(!gifRunning){
		LoopManager::removeListener(this);
		return;
	}

	if(millis() - frameTime < 100) return;
	nextFrame();
}

void BoostElement::nextFrame(){
	frame = (frame+1) % 12;
	frameTime = millis();

	String path = "S:/Fire/frame" + String(frame) + ".bin";
	lv_img_set_src(bgIMG, path.c_str());
}
