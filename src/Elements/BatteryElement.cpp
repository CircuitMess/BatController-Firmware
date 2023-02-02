#include <cstdio>
#include <Loop/LoopManager.h>
#include "BatteryElement.h"

BatteryElement::BatteryElement(lv_obj_t* parent, BatType type) : LVObject(parent){
	img = lv_img_create(obj);
	lv_obj_set_size(img, 12, 5);
	lv_obj_align(img, LV_ALIGN_CENTER, 15, 0);
	setLevel(7);

	device = lv_img_create(obj);
	lv_obj_align(device, LV_ALIGN_CENTER, 0, 0);
	char imgPath[50];
	if(type == BatType::Batmobile){
		sprintf(imgPath, "S:/Battery/Batmobile.bin");
	}else{
		sprintf(imgPath, "S:/Battery/Controller.bin");
	}
	lv_img_set_src(device, imgPath);
}

BatteryElement::~BatteryElement(){
	LoopManager::removeListener(this);
}

void BatteryElement::setLevel(uint8_t index){
	char imgPath[50];
	sprintf(imgPath, "S:/Battery/%d.bin", index);
	lv_img_set_src(img, imgPath);
	picIndex = index;
}

void BatteryElement::setCharging(bool charging){
	if(this->charging == charging) return; // we don't want to add the same listener multiple times
	this->charging = charging;

	if(charging){
		chargeTimer = 0;
	}

	if(blinking || charging){
		LoopManager::addListener(this);
	}else{
		LoopManager::removeListener(this);
	}
}

void BatteryElement::setBlinking(bool blinking){
	if(this->blinking == blinking) return;
	this->blinking = blinking;

	if(blinking){
		hidden = false;
		blinkTimer = 0;
	}else{
		lv_obj_clear_flag(img, LV_OBJ_FLAG_HIDDEN);
	}

	if(blinking || charging){
		LoopManager::addListener(this);
	}else{
		LoopManager::removeListener(this);
	}
}


void BatteryElement::loop(uint micros){
	if(!charging && !blinking){
		LoopManager::removeListener(this);
	}

	if(blinking){
		blinkTimer += micros;
		if(blinkTimer >= BlinkInterval * 1000){
			blinkTimer = 0;
			hidden = !hidden;

			if(hidden){
				lv_obj_add_flag(img, LV_OBJ_FLAG_HIDDEN);
			}else{
				lv_obj_clear_flag(img, LV_OBJ_FLAG_HIDDEN);
			}
		}
	}

	if(charging){
		chargeTimer += micros;
		if(chargeTimer >= ChargeInterval){
			chargeTimer = 0;

			setLevel((picIndex + 1) % 8);
		}
	}
}
