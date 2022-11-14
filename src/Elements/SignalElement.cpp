#include <cstdio>
#include <Com/Communication.h>
#include "SignalElement.h"

SignalElement::SignalElement(lv_obj_t* parent) : LVObject(parent){
	img = lv_img_create(obj);
	lv_obj_set_size(img, 9, 5);
	lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);

	setLevel(0);
	Com.addListener(this);
}

void SignalElement::setLevel(uint8_t index){
	char imgPath[50];
	signalStrength = index;
	sprintf(imgPath, "S:/Signal/%d.bin", signalStrength);
	lv_img_set_src(img, imgPath);
}

SignalElement::~SignalElement(){
	Com.removeListener(this);
}

void SignalElement::onSignalStrength(uint8_t percent){
	uint8_t locIndex;
	if(percent >= 75){
		locIndex = 3;
	}else if(percent >= 50){
		locIndex = 2;
	}else if(percent >= 25){
		locIndex = 1;
	}else{
		locIndex = 0;
	}
	if(signalStrength != locIndex) setLevel(locIndex);
}
