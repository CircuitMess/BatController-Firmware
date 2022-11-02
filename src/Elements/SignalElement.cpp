#include <cstdio>
#include "SignalElement.h"

SignalElement::SignalElement(lv_obj_t* parent) : LVObject(parent){
	img = lv_img_create(obj);
	lv_obj_set_size(img, 9, 5);
	lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);

	setLevel(3);

}

void SignalElement::setLevel(uint8_t index){
	char imgPath[50];
	sprintf(imgPath, "S:/Signal/%d.bin", index);
	lv_img_set_src(img, imgPath);
}
