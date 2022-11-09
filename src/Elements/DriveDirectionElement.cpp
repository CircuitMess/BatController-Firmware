#include <cstdlib>
#include <Arduino.h>
#include "DriveDirectionElement.h"
#include <fstream>
#include <iostream>
#include <SPIFFS.h>
#include <Display/Color.h>

DriveDirectionElement::DriveDirectionElement(lv_obj_t* parent) : LVObject(parent){
	bufferMain = static_cast<lv_color_t*>(malloc(sizeof(lv_color_t) * 15 * 15)); //W:15*H:15
	canvasMain = lv_canvas_create(obj);
	lv_canvas_set_buffer(canvasMain, bufferMain, 15, 15, LV_IMG_CF_TRUE_COLOR);
	lv_canvas_fill_bg(canvasMain, lv_color_make(255, 255, 255), LV_OPA_COVER);

	bufferHelper = static_cast<lv_color_t*>(malloc(sizeof(lv_color_t) * 15 * 13)); //W:15*H:13
	canvasHelper = lv_canvas_create(obj);
	lv_canvas_set_buffer(canvasHelper, bufferHelper, 15, 13, LV_IMG_CF_TRUE_COLOR);
	lv_obj_add_flag(canvasHelper, LV_OBJ_FLAG_HIDDEN);

	setSpeed(0);
	setDirection(0);
}

DriveDirectionElement::~DriveDirectionElement(){
	free(bufferMain);
	free(bufferHelper);
}

void DriveDirectionElement::setSpeed(uint8_t speed){
	this->speed = constrain(speed, (uint8_t)0,  (uint8_t)100);
	drawFill();
	drawRotation();
}

void DriveDirectionElement::setDirection(float angle){
	if(angle <0 || angle > 360){
		angle = fmod(angle, 360.0f);
		if(angle < 0) angle += 360.0f;
	}
	this->angle = angle;
	drawRotation();
}

void DriveDirectionElement::drawFill(){
	auto drawToCanvas = [this](const char* path, uint8_t startingY = 0){
		File f = SPIFFS.open(path);
		if(!f){
			Serial.printf("Could not open file %s\n", path);
			return;
		}

		uint8_t buf[390];
		f.read(buf, f.size());

		for(uint8_t x = 0; x < 15; x++){
			for(uint8_t y = startingY; y < 13; y++){
				if(((uint16_t*) buf)[y * 15 + x] == 0)
					lv_canvas_set_px(canvasHelper, x, y, lv_color_black());
			}
		}
	};

	lv_canvas_fill_bg(canvasHelper, lv_color_make(255, 255, 255), LV_OPA_0);
	drawToCanvas(arrowEmptyPath);
	uint8_t percent = 13 - 13 * speed / 100;
	drawToCanvas(arrowFullPath, percent);

	lv_obj_invalidate(obj);
}

void DriveDirectionElement::drawRotation(){
	lv_canvas_transform(canvasMain, lv_canvas_get_img(canvasHelper), (int16_t) (angle * 10), 256, 0, 0, 15 / 2, 15 / 2, true);
	lv_obj_invalidate(obj);
}