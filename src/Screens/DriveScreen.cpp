#include "DriveScreen.h"
#include <Input/Input.h>
#include <BatController.h>
#include <DriveMode.h>
#include "../Driver/ManualDriver.h"
#include "../Driver/BallDriver.h"
#include "PairScreen.h"
#include <Com/Communication.h>

DriveScreen::DriveScreen(DriveMode mode) : LVScreen(), infoElement(obj, mode){
	lv_obj_add_flag(infoElement.getLvObj(), LV_OBJ_FLAG_IGNORE_LAYOUT);
	lv_obj_set_pos(infoElement.getLvObj(), 0, 0);

	img = lv_img_create(obj);
	lv_obj_set_size(img, 160, 120);
	lv_obj_add_flag(img, LV_OBJ_FLAG_IGNORE_LAYOUT);
	lv_obj_set_pos(img, 0, 8);

	imgBuf = static_cast<Color*>(heap_caps_malloc(160 * 120 * 2, MALLOC_CAP_INTERNAL | MALLOC_CAP_32BIT));
	imgDsc.data = (const uint8_t*) imgBuf;
	imgDsc.data_size = 160 * 120 * 2;
	imgDsc.header.w = 160;
	imgDsc.header.h = 120;
	imgDsc.header.cf = LV_IMG_CF_TRUE_COLOR;
	imgDsc.header.always_zero = 0;
	lv_img_set_src(img, &imgDsc);

	driverLayer = lv_obj_create(obj);
	lv_obj_add_flag(driverLayer, LV_OBJ_FLAG_IGNORE_LAYOUT);
	lv_obj_set_size(driverLayer, lv_obj_get_width(obj), lv_obj_get_height(obj));
	lv_obj_move_foreground(driverLayer);

	feed.onFrame([this](const DriveInfo& info, const Color* frame){
		if(!isRunning()) return;

		memcpy(imgBuf, frame, 160 * 120 * 2);

		if(driver){
			driver->onFrame(info, imgBuf);
		}

		lv_obj_invalidate(img);
	});

	// If mode is idle, do nothing (setMode returns early)
	setMode(mode);
}

DriveScreen::~DriveScreen(){
	free(imgBuf);
}

void DriveScreen::onStarting(){
	memset(imgBuf, 0xff, 160 * 120 * 2);
}

void DriveScreen::onStart(){
	driver->start();
	Input::getInstance()->addListener(this);
	Com.addDcListener(this);
}

void DriveScreen::onStop(){
	driver->stop();
	Input::getInstance()->removeListener(this);
	Com.removeDcListener(this);
}

void DriveScreen::setMode(DriveMode newMode){
	if(newMode == currentMode) return;

	driver.reset();

	static const std::map<DriveMode, std::function<std::unique_ptr<Driver>(lv_obj_t* elementContainer, LVScreen* screen)>> starter = {
			{ DriveMode::Manual, [](lv_obj_t* elementContainer, LVScreen* screen){ return std::make_unique<ManualDriver>(elementContainer); }},
			{ DriveMode::Ball,   [](lv_obj_t* elementContainer, LVScreen* screen){ return std::make_unique<BallDriver>(elementContainer, screen); }},
			{ DriveMode::Marker, [](lv_obj_t* elementContainer, LVScreen* screen){ return nullptr; }},
			{ DriveMode::Line,   [](lv_obj_t* elementContainer, LVScreen* screen){ return nullptr; }}
	};

	if(driver == nullptr){
		currentMode = DriveMode::Idle;
		return;
	}

	driver = starter.at(newMode)(driverLayer, this);
	currentMode = newMode;
	Com.sendDriveMode(currentMode);
}

void DriveScreen::buttonPressed(uint i){
	switch(i){
		case BTN_B:
			Com.sendHonk();
			break;
		case BTN_MENU:
			pop();
			break;
	}
}

void DriveScreen::onDisconnected(){
	stop();
	delete this;

	auto pair = new PairScreen();
	pair->start();
}
