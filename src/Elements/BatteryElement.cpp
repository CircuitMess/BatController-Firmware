#include <cstdio>
#include <Loop/LoopManager.h>
#include "BatteryElement.h"

BatteryElement::BatteryElement(lv_obj_t* parent, BatType type) : LVObject(parent){
	img = lv_img_create(obj);
	lv_obj_set_size(img, 12, 5);
	lv_obj_align(img, LV_ALIGN_CENTER, 14, 0);
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

void BatteryElement::setLevel(uint8_t index){
	char imgPath[50];
	sprintf(imgPath, "S:/Battery/%d.bin", index);
	lv_img_set_src(img, imgPath);
}

void BatteryElement::setCharging(bool charging){
	if(this->charging == charging) return; // we don't want to add the same listener multiple times
	this->charging = charging;
	if(charging){
		LoopManager::addListener(this);
	}else{
		LoopManager::removeListener(this);
	}
}

void BatteryElement::loop(uint micros){
	microCounter += micros;
	if(microCounter >= checkInterval){
		microCounter = 0;
		setLevel(picIndex);
		picIndex++;
		if(picIndex == 8) picIndex = 0;
	}
}

BatteryElement::~BatteryElement(){
	setCharging(false);
}
