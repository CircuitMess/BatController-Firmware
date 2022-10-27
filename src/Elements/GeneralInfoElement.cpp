#include <cstdio>
#include "GeneralInfoElement.h"

GeneralInfoElement::GeneralInfoElement(lv_obj_t* parent, DriveMode mode) : LVObject(parent){
	lv_obj_set_pos(obj, 0, 0);
	lv_obj_set_size(obj, 160, 10);
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
}

void GeneralInfoElement::setMode(DriveMode mode){
	char imgPath[50];
	switch(mode){
		case Manual:
			sprintf(imgPath, "S:/DriveMode/Manual.bin");
			lv_obj_set_size(modeImg, 30, 5);
			break;
		case Ball:
			sprintf(imgPath, "S:/DriveMode/Ball.bin");
			lv_obj_set_size(modeImg, 57, 5);
			break;
		case Marker:
			sprintf(imgPath, "S:/DriveMode/Marker.bin");
			lv_obj_set_size(modeImg, 69, 5);
			break;
		case Line:
			sprintf(imgPath, "S:/DriveMode/Line.bin");
			lv_obj_set_size(modeImg, 55, 5);
			break;
		case Idle:
			sprintf(imgPath, "S:/DriveMode/Idle.bin");
			lv_obj_set_size(modeImg, 14, 5);
			break;
	}
	lv_img_set_src(modeImg, imgPath);
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
