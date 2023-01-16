#include <cstdio>
#include "GeneralInfoElement.h"
#include <Loop/LoopManager.h>
#include <BatteryService.h>
#include <Com/Communication.h>

const char* GeneralInfoElement::modePaths[] = {
		"",
		"S:/DriveMode/Manual.bin",
		"S:/DriveMode/Ball.bin",
		"S:/DriveMode/Line.bin",
		"S:/DriveMode/Marker.bin",
		"",
		"S:/DriveMode/SimpleProg.bin",
};

GeneralInfoElement::GeneralInfoElement(lv_obj_t* parent, DriveMode mode) : LVObject(parent){
	lv_obj_set_pos(obj, 0, 0);
	lv_obj_set_size(obj, 160, 8);
	lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), 0);
	lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, 0);
	lv_obj_align(obj, LV_ALIGN_TOP_MID, 0, 0);

	modeImg = lv_img_create(obj);
	lv_obj_align(modeImg, LV_ALIGN_CENTER, 0, 0);
	setMode(mode);

	signal = new SignalElement(obj);
	lv_obj_align(signal->getLvObj(), LV_ALIGN_LEFT_MID, -30, 0);

	conBat = new BatteryElement(obj, BatType::Controller);
	lv_obj_align(conBat->getLvObj(), LV_ALIGN_RIGHT_MID, 40, 0);
	mobBat = new BatteryElement(obj, BatType::Batmobile);
	lv_obj_align(mobBat->getLvObj(), LV_ALIGN_LEFT_MID, -57, 0);

	conBatIndex = Battery.getLevel();
	conBat->setLevel(conBatIndex);

	Com.addListener(this);
	LoopManager::addListener(this);
}

GeneralInfoElement::~GeneralInfoElement(){
	LoopManager::removeListener(this);
	Com.removeListener(this);
}

void GeneralInfoElement::setMode(DriveMode mode){
	lv_img_set_src(modeImg, modePaths[(int) mode]);
}

void GeneralInfoElement::loop(uint micros){
	counter += micros;
	if(counter >= BatteryCheckInterval){
		counter = 0;
		if(Battery.charging()){
			conBat->setCharging(true);
			return;
		} else {
			conBat->setCharging(false);
			if(conBatIndex == Battery.getLevel()) return;
			conBatIndex = Battery.getLevel();
			conBat->setLevel(conBatIndex);
		}
	}
}

void GeneralInfoElement::onBattery(uint8_t percent, bool charging){
	if(charging){
		mobBat->setCharging(true);
	}else{
		mobBat->setCharging(false);
		if(percent > 90){
			mobBat->setLevel(7);
		}else if(percent > 78){
			mobBat->setLevel(6);
		}else if(percent > 66){
			mobBat->setLevel(5);
		}else if(percent > 54){
			mobBat->setLevel(4);
		}else if(percent > 42){
			mobBat->setLevel(3);
		}else if(percent > 30){
			mobBat->setLevel(2);
		}else if(percent >= 10){
			mobBat->setLevel(1);
		}else if(percent < 10){
			mobBat->setLevel(0);
		}
	}
}