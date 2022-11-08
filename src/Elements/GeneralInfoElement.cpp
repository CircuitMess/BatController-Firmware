#include <cstdio>
#include "GeneralInfoElement.h"
#include <Loop/LoopManager.h>
#include <BatteryService.h>

const char* GeneralInfoElement::modePaths[] = {
		"S:/DriveMode/Idle.bin",
		"S:/DriveMode/Manual.bin",
		"S:/DriveMode/Ball.bin",
		"S:/DriveMode/Line.bin",
		"S:/DriveMode/Marker.bin"
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
	setConBat(conBatIndex);

	LoopManager::addListener(this);
}

GeneralInfoElement::~GeneralInfoElement(){
	LoopManager::removeListener(this);
}

void GeneralInfoElement::setMode(DriveMode mode){
	lv_img_set_src(modeImg, modePaths[(int) mode]);
}


void GeneralInfoElement::setSignal(uint8_t index){
	signal->setLevel(index);
}

void GeneralInfoElement::setConBat(uint8_t index){
	conBat->setLevel(index);
}

void GeneralInfoElement::setMobBat(uint8_t index){
	mobBat->setLevel(index);
}

void GeneralInfoElement::loop(uint micros){
	counter += micros;
	if(counter >= BatteryCheckInterval){
		counter = 0;
		if(conBatIndex == Battery.getLevel()) return;
		conBatIndex = Battery.getLevel();
		setConBat(conBatIndex);
	}
}
